#include "pvarmenu.h"
#include <QMessageBox>
#include "guiUtils.h"
#include "pvarscell.h"
#include "ui_pvarmenu.h"
using namespace std;
using namespace LongQt;

PvarMenu::PvarMenu(shared_ptr<Protocol> proto, QWidget *parent)
    : QWidget(parent), ui(new Ui::PvarMenu) {
  ui->setupUi(this);
  this->proto = proto;
  this->pvarsDescriptions = GuiUtils::readDesc(":/hoverText/cellParsDesc.json");
  ui->treeWidget->addAction(ui->actionDelete);
  ui->treeWidget->addAction(ui->actionShow_Cells);
  this->numTrials = new SimvInt(this->proto, "numtrials", ui->numTrialsSpinBox);
  this->numTrials->setupLabel(ui->numTrialsLabel);
  this->numTrials->update_ui();
  this->updateList();
}

PvarMenu::~PvarMenu() { delete ui; }

void PvarMenu::updateList() {
  this->ui->treeWidget->clear();
  for (auto &pvar : this->proto->pvars()) {
    auto temp = new QTreeWidgetItem(
        ui->treeWidget,
        QString(pvar.second->IonChanParam::str(pvar.first).c_str())
            .split("\t"));
    temp->setData(0, Qt::ToolTipRole,
                  pvarsDescriptions[pvar.first.c_str()]["Description"]);
  }
}

void PvarMenu::hideNumTrials(bool hide) {
  if (hide) {
    ui->numTrialsSpinBox->hide();
    ui->numTrialsLabel->hide();
  } else {
    ui->numTrialsSpinBox->show();
    ui->numTrialsLabel->show();
  }
}

void PvarMenu::changeProto(shared_ptr<Protocol> proto) {
  this->proto = proto;
  if (this->addmenu) addmenu->changeProto(proto);
  this->numTrials->changeProto(this->proto);
  this->numTrials->update_ui();
  this->updateList();
}

void PvarMenu::changeCell(shared_ptr<Cell> cell) {
  if (this->addmenu) addmenu->changeCell(cell);
  this->numTrials->changeCell(cell);
  this->numTrials->update_ui();
  this->updateList();
}

void PvarMenu::on_actionDelete_triggered() {
  QList<QTreeWidgetItem *> items = ui->treeWidget->selectedItems();
  for (auto item : items) {
    QString name = item->data(0, Qt::DisplayRole).toString();
    this->proto->pvars().erase(name.toStdString());
    int row = ui->treeWidget->indexOfTopLevelItem(item);
    ui->treeWidget->takeTopLevelItem(row);
    delete item;
  }
}

void PvarMenu::on_actionShow_Cells_triggered() {
  QList<QTreeWidgetItem *> items = ui->treeWidget->selectedItems();
  QString text;
  for (auto item : items) {
    QString name = item->data(0, Qt::DisplayRole).toString();
    text += this->proto->pvars()
                .at(name.toStdString())
                ->str(name.toStdString())
                .c_str();
  }
  QMessageBox msgBox;
  msgBox.setText(text);
  msgBox.exec();
}

void PvarMenu::on_addButton_triggered() {
  QList<QTreeWidgetItem *> items = ui->treeWidget->selectedItems();
  PvarsCell::IonChanParam *param = NULL;
  QString name;
  if (items.length() > 0) {
    name = items[0]->data(0, 0).toString();
    param = this->proto->pvars().at(name.toStdString());
  }
  this->addmenu = new AddSingleCellPvar(this->proto, {name, param}, this);
  connect(addmenu, &AddSingleCellPvar::pvarsChanged,
          [this]() { this->updateList(); });
  addmenu->show();
}

void PvarMenu::on_treeWidget_itemSelectionChanged() {
  QList<QTreeWidgetItem *> items = ui->treeWidget->selectedItems();
  if (!items.empty()) {
    ui->infoButton->setEnabled(true);
  } else {
    ui->infoButton->setEnabled(false);
  }
}

void PvarMenu::on_removeButton_triggered() {
  this->on_actionDelete_triggered();
}

void PvarMenu::on_infoButton_triggered() {
  this->on_actionShow_Cells_triggered();
}

void PvarMenu::on_refreshButton_triggered() {
  this->proto->pvars().calcIonChanParams();
}
