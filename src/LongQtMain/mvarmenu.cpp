#include "mvarmenu.h"
#include "ui_mvarmenu.h"
#include "guiUtils.h"

#include <QDebug>
#include <QScopedPointer>
#include <QTreeWidget>
#include <QSpinBox>
#include <QRegExp>

/*#################################
  begin mvarMenu class
###################################*/
MvarMenu::MvarMenu(shared_ptr<Protocol> proto, QWidget *parent):
QWidget(parent),
ui(new Ui::MvarMenu)
{
    ui->setupUi(this);
    //setup class variables
    this->proto = proto;

    auto type = proto->cell()->type();
    this->dvarsDescriptions = GuiUtils::concatMaps(
            GuiUtils::readMap(":/hoverText/dvarsDescriptions.json", type),
            ", ",GuiUtils::readMap(":/hoverText/dvarsUnits.json", type));
    this->measDescriptions = GuiUtils::readMap(":/hoverText/measDescriptions.json", type);
    this->setupMenu();
    connect(ui->measView,
        static_cast<void(QTreeWidget::*)(QTreeWidgetItem*, int)>(
            &QTreeWidget::itemChanged),
        this,
        static_cast<void(MvarMenu::*)(QTreeWidgetItem*, int)>(
            &MvarMenu::addMeas));
    connect(ui->percrepolBox,
        static_cast<void(QSpinBox::*)(int)>(&QSpinBox::valueChanged),
        [this](int val) {
            this->proto->measureMgr().percrepol(val);
        });
}

MvarMenu::~MvarMenu(){}

void MvarMenu::setupMenu()  {
    int cellType = 0;
    QMap<string,int> measIds;
    set<string> measOptionsDefault = Measure().variables();
    for(auto& cellVar: this->proto->cell()->getVariables()) {
        this->cellVars.append(cellVar.c_str());
        auto cellItem = new QTreeWidgetItem(
            ui->measView,
            {cellVar.c_str(), this->getType(cellVar.c_str())},
            cellType);
        cellItem->setData(0,Qt::ToolTipRole,this->dvarsDescriptions[cellVar.c_str()]);
        set<string> measOptions;
        if(proto->measureMgr().varsMeas.count(cellVar)>0) {
            string measureName = proto->measureMgr().varsMeas.at(cellVar);
            QScopedPointer<Measure> m(
                proto->measureMgr().varMeasCreator.at(measureName)({}));
            measOptions = m->variables();
        } else {
            measOptions = measOptionsDefault;
        }
        auto selection = proto->measureMgr().selection();
        for(auto& measVar: measOptions) {
            if(!measIds.contains(measVar.c_str())) {
                int id =measIds.size();
                measIds[measVar] = id;
                this->measVars.append(measVar);
            }
            auto measItem = new QTreeWidgetItem(
                cellItem,
                {measVar.c_str()},
                measIds[measVar]);
                measItem->setData(0,Qt::ToolTipRole,this->measDescriptions[measVar.c_str()]);
            if(selection.count(cellVar)>0 &&
                selection.at(cellVar).count(measVar)>0) {
                measItem->setCheckState(0,Qt::Checked);
            } else {
                measItem->setCheckState(0,Qt::Unchecked);
            }
        }
        this->setParentCheckedState(cellItem,0);
        ++cellType;
    }
    ui->percrepolBox->setValue(proto->measureMgr().percrepol());
}
void MvarMenu::reset() {
    this->cellVars.clear();
    this->measVars.clear();
    ui->measView->clear();
    bool oldState = ui->measView->blockSignals(true);
    setupMenu();
    ui->measView->blockSignals(oldState);
}
void MvarMenu::changeProto(shared_ptr<Protocol> proto) {
    this->proto = proto;
    this->reset();
}
void MvarMenu::changeCell(shared_ptr<Cell> cell) {
    if(cell != proto->cell()) {
        qWarning("DvarMenu: Cell is not the same as proto's cell");
    }
    this->reset();
}
void MvarMenu::addMeas(QTreeWidgetItem * item, int column) {
    if(item->parent() == NULL) {
        this->parentMeas(item, column);
    } else {
        this->childMeas(item, column);
    }
}
void MvarMenu::childMeas(QTreeWidgetItem * item, int column) {
    bool state = item->checkState(column);
    string measVar = this->measVars[item->type()];
    string cellVar = this->cellVars[item->parent()->type()];
    auto sel = this->proto->measureMgr().selection();
    auto measSel = sel[cellVar];
    if(state) {
        measSel.insert(measVar);
    } else {
        measSel.erase(measVar);
    }
    sel[cellVar] = measSel;
    if(sel[cellVar].empty()) {
        sel.erase(cellVar);
    }
    proto->measureMgr().selection(sel);
    this->setParentCheckedState(item->parent(),column);
}
void MvarMenu::parentMeas(QTreeWidgetItem * item, int column) {
    auto state = item->checkState(column);
    string cellVar = this->cellVars[item->type()];
    this->setChildrenCheckedStates(item, column, state);
}
void MvarMenu::setParentCheckedState(QTreeWidgetItem* item, int column) {
    int numChildChecked = 0;
    for(int i = 0; i < item->childCount(); ++i) {
        auto child = item->child(i);
        if(child->checkState(column)==Qt::Checked) {
            ++numChildChecked;
        }
    }
    if(item->childCount() == numChildChecked) {
        item->setCheckState(column,Qt::Checked);
    } else if(numChildChecked>0) {
        item->setCheckState(column,Qt::PartiallyChecked);
    } else {
        item->setCheckState(column,Qt::Unchecked);
    }
}
void MvarMenu::setChildrenCheckedStates(QTreeWidgetItem* item, int column, Qt::CheckState state) {
    if(state == Qt::PartiallyChecked) return;
    for(int i = 0; i < item->childCount(); ++i) {
        item->child(i)->setCheckState(column, state);
    }
}

QString MvarMenu::getType(QString name) {
    QMap<QString,QRegExp> groups =
        {{"Gates",QRegExp("^Gate.",Qt::CaseInsensitive)},
        {"Currents",QRegExp("^i")},
        {"Calcium Concentrations", QRegExp("^ca")}};
    for(auto group = groups.begin(); group != groups.end(); ++group) {
        if(group->indexIn(name) != -1) {
            return group.key();
        }
    }
    return "Other";
}
