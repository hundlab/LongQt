#include "addgridcellpvar.h"
#include "guiUtils.h"
#include "pvarscell.h"
#include "ui_addgridcellpvar.h"
using namespace std;
using namespace LongQt;

AddGridCellPvar::AddGridCellPvar(QTableView* view,
                                 shared_ptr<GridProtocol> proto,
                                 QWidget* parent)
    : QDialog(parent), ui(new Ui::AddGridCellPvar) {
  ui->setupUi(this);

  this->view = view;
  this->proto = proto;
  this->updateIonChannelType();
  this->pvarsDescriptions = GuiUtils::readDesc(":/hoverText/cellParsDesc.json");
}

AddGridCellPvar::~AddGridCellPvar() { delete ui; }

void AddGridCellPvar::updateIonChannelType() {
  ui->ionChannelType->clear();
  int pos = 0;
  for (auto& pvarName : this->proto->cell()->pars()) {
    if (this->pvarsDescriptions.contains(pvarName.c_str())) {
      ui->ionChannelType->addItem(pvarName.c_str());
      auto& desc = this->pvarsDescriptions[pvarName.c_str()];
      auto tip = desc["Description"];
      if (desc.contains("Units")) {
        tip += ", " + desc["Units"];
      }
      ui->ionChannelType->setItemData(pos, tip, Qt::ToolTipRole);
      ++pos;
    }
  }
}

void AddGridCellPvar::on_randomize_stateChanged(int state) {
  ui->normalDist->setVisible(state);
  ui->lognormalDist->setVisible(state);
  ui->mean->setVisible(state);
  ui->meanLabel->setVisible(state);
  ui->stdDev->setVisible(state);
  ui->stdDevLabel->setVisible(state);
  ui->startVal->setVisible(!state);
  ui->startValLabel->setVisible(!state);
  ui->incAmt->setVisible(!state);
  ui->incAmtLabel->setVisible(!state);
}

void AddGridCellPvar::on_multiple_stateChanged(int state) {
  ui->maxDist->setValue(0);
  ui->maxVal->setValue(1000);
  ui->maxDist->setEnabled(state);
  ui->maxDistLabel->setEnabled(state);
  ui->maxVal->setEnabled(state);
  ui->maxValLabel->setEnabled(state);
}

void AddGridCellPvar::on_normalDist_toggled(bool checked) {
  if (checked) {
    if (ui->mean->value() == 0) {
      ui->mean->setValue(1);
    }
  } else {
    if (ui->mean->value() == 1) {
      ui->mean->setValue(0);
    }
  }
}
void AddGridCellPvar::on_addButton_clicked() {
  string type = ui->ionChannelType->currentText().toStdString();
  PvarsCell::IonChanParam toAdd = PvarsCell::IonChanParam();
  if (ui->randomize->checkState() == 0) {
    toAdd.dist = PvarsCell::Distribution::none;
    toAdd.val[0] = ui->startVal->value();
    toAdd.val[1] = ui->incAmt->value();
  } else {
    toAdd.val[0] = ui->mean->value();
    toAdd.val[1] = ui->stdDev->value();
    if (ui->normalDist->isChecked()) {
      toAdd.dist = PvarsCell::Distribution::normal;
    } else {
      toAdd.dist = PvarsCell::Distribution::lognormal;
    }
  }
  proto->pvars().insert(type, toAdd);
  if (ui->multiple->isChecked()) {
    int maxDist = ui->maxDist->value();
    double maxVal = ui->maxVal->value();
    ((PvarsGrid*)&proto->pvars())->setMaxDistAndVal(type, maxDist, maxVal);
  }
  auto startCells = this->getInitial();
  ((PvarsGrid*)&proto->pvars())->setStartCells(type, startCells);
  emit pvarsChanged();
}
set<pair<int, int>> AddGridCellPvar::getInitial() {
  set<pair<int, int>> startCells;
  auto selected = this->view->selectionModel()->selectedIndexes();
  for (auto toAdd : selected) {
    startCells.insert({toAdd.row(), toAdd.column()});
  }
  return startCells;
}
void AddGridCellPvar::changeProto(shared_ptr<GridProtocol> proto) {
  this->proto = proto;
}

void AddGridCellPvar::changeCell(shared_ptr<Cell>) {
  this->updateIonChannelType();
}
