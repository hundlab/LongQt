#include "mvarmenu.h"
#include "guiUtils.h"
#include "ui_mvarmenu.h"

#include <logger.h>
#include <measuredefault.h>
#include <QRegExp>
#include <QScopedPointer>
#include <QSpinBox>
#include <QTreeWidget>
using namespace std;
using namespace LongQt;

/*#################################
  begin mvarMenu class
###################################*/
MvarMenu::MvarMenu(shared_ptr<Protocol> proto, QWidget* parent)
    : QWidget(parent), ui(new Ui::MvarMenu) {
  ui->setupUi(this);
  // setup class variables
  this->proto = proto;
  this->model = new DataOutputSelectionModel(this->proto, this);
  ui->measView->setModel(this->model);
//  ui->measView->setItemDelegate(
//      new DataOutputSelectionDelegate(this->ui->measView));
  //  auto type = proto->cell()->type();
  //  this->dvarsDescriptions = GuiUtils::concatMaps(
  //      GuiUtils::readMap(":/hoverText/dvarsDescriptions.json", type), ",
  //      ", GuiUtils::readMap(":/hoverText/dvarsUnits.json", type));
  //  this->measDescriptions =
  //      GuiUtils::readMap(":/hoverText/measDescriptions.json", type);
  //  this->setupMenu();
  //  connect(ui->measView,
  //          static_cast<void (QTreeWidget::*)(QTreeWidgetItem*, int)>(
  //              &QTreeWidget::itemChanged),
  //          this,
  //          static_cast<void (MvarMenu::*)(QTreeWidgetItem*, int)>(
  //              &MvarMenu::addMeas));
  connect(ui->percrepolBox,
          static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),
          [this](int val) { this->proto->measureMgr().percrepol(val); });
}

MvarMenu::~MvarMenu() {}

void MvarMenu::setupMenu() {
  //  int cellVarID = 0;
  //  QMap<string, int> measIds;
  //  auto& measMaker = this->proto->measureMgr().measMaker;
  //  for (auto& cellVar : this->proto->cell()->vars()) {
  //    this->cellVars.append(cellVar.c_str());
  //    auto cellItem = new QTreeWidgetItem(
  //        ui->measView, {cellVar.c_str(), "", "",
  //        this->getType(cellVar.c_str())}, cellVarID);
  //    cellItem->setData(0, Qt::ToolTipRole,
  //                      this->dvarsDescriptions[cellVar.c_str()]);
  //    {
  //      set<string> measOptions =
  //          measMaker.measureOptions(measMaker.measureType(cellVar));
  //      auto selection = proto->measureMgr().selection();
  //      for (auto& measVar : measOptions) {
  //        if (!measIds.contains(measVar.c_str())) {
  //          int id = measIds.size();
  //          measIds[measVar] = id;
  //          this->measVars.append(measVar);
  //        }
  //        auto measItem = new QTreeWidgetItem(cellItem, {"", measVar.c_str()},
  //                                            measIds[measVar]);
  //        measItem->setData(0, Qt::ToolTipRole,
  //                          this->measDescriptions[measVar.c_str()]);
  //        if (selection.count(cellVar) > 0 &&
  //            selection.at(cellVar).count(measVar) > 0) {
  //          measItem->setCheckState(1, Qt::Checked);
  //        } else {
  //          measItem->setCheckState(1, Qt::Unchecked);
  //        }
  //      }
  //      //    this->setParentCheckedState(cellItem, 1);
  //    }
  //    {
  //      auto selection = this->proto->cell()->getVariableSelection();
  //      if (selection.count(cellVar)) {
  //        cellItem->setCheckState(2, Qt::Checked);
  //      } else {
  //        cellItem->setCheckState(2, Qt::Unchecked);
  //      }
  //      if (cellVar == "t" || cellVar == "vOld") {
  //        cellItem->setDisabled(true);
  //      }
  //    }
  //    ++cellVarID;
  //  }
  ui->percrepolBox->setValue(proto->measureMgr().percrepol());
}
void MvarMenu::reset() {
  this->cellVars.clear();
  this->measVars.clear();
  bool oldState = ui->measView->blockSignals(true);
  setupMenu();
  ui->measView->blockSignals(oldState);
}
void MvarMenu::changeProto(shared_ptr<Protocol> proto) {
  this->proto = proto;
  this->model->changeProto(proto);
  this->reset();
}
void MvarMenu::changeCell(shared_ptr<Cell> cell) {
  if (cell != proto->cell()) {
    Logger::getInstance()->write(
        "DvarMenu: Cell is not the same as proto's cell");
  }
  this->model->changeCell(cell);
  this->reset();
}
void MvarMenu::addMeas(QTreeWidgetItem* item, int column) {
  if (item->parent() == NULL) {
    this->parentMeas(item, column);
  } else {
    this->childMeas(item, column);
  }
}
void MvarMenu::childMeas(QTreeWidgetItem* item, int column) {
  bool state = item->checkState(column);
  string measVar = this->measVars[item->type()];
  string cellVar = this->cellVars[item->parent()->type()];
  auto sel = this->proto->measureMgr().selection();
  auto measSel = sel[cellVar];
  if (state) {
    measSel.insert(measVar);
  } else {
    measSel.erase(measVar);
  }
  sel[cellVar] = measSel;
  if (sel[cellVar].empty()) {
    sel.erase(cellVar);
  }
  proto->measureMgr().selection(sel);
  this->setParentCheckedState(item->parent(), column);
}
void MvarMenu::parentMeas(QTreeWidgetItem* item, int column) {
  auto state = item->checkState(column);
  string cellVar = this->cellVars[item->type()];
  this->setChildrenCheckedStates(item, column, state);
}
void MvarMenu::setParentCheckedState(QTreeWidgetItem* item, int column) {
  int numChildChecked = 0;
  for (int i = 0; i < item->childCount(); ++i) {
    auto child = item->child(i);
    if (child->checkState(column) == Qt::Checked) {
      ++numChildChecked;
    }
  }
  if (item->childCount() == numChildChecked) {
    item->setCheckState(column, Qt::Checked);
  } else if (numChildChecked > 0) {
    item->setCheckState(column, Qt::PartiallyChecked);
  } else {
    item->setCheckState(column, Qt::Unchecked);
  }
}
void MvarMenu::setChildrenCheckedStates(QTreeWidgetItem* item, int column,
                                        Qt::CheckState state) {
  if (state == Qt::PartiallyChecked) return;
  for (int i = 0; i < item->childCount(); ++i) {
    item->child(i)->setCheckState(column, state);
  }
}
