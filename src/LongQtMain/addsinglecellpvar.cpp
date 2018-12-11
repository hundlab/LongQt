#include "addsinglecellpvar.h"
#include "guiUtils.h"
#include "pvarscell.h"
#include "ui_addsinglecellpvar.h"
using namespace std;
using namespace LongQt;

AddSingleCellPvar::AddSingleCellPvar(
    shared_ptr<Protocol> proto,
    std::pair<QString, PvarsCell::IonChanParam*> parampair, QWidget* parent)
    : QDialog(parent), ui(new Ui::AddSingleCellPvar) {
  ui->setupUi(this);

  this->proto = proto;
  this->pvarsDescriptions = GuiUtils::readMap(
      ":/hoverText/pvarsDescriptions.json", proto->cell()->type());

  this->updateIonChannelType();
  this->setCurrentSelect(parampair.first, parampair.second);
}

AddSingleCellPvar::~AddSingleCellPvar() { delete ui; }

void AddSingleCellPvar::setCurrentSelect(QString name,
                                         PvarsCell::IonChanParam* param) {
  if (param == NULL) return;

  ui->ionChannelType->setCurrentText(name);
  switch (param->dist) {
    case PvarsCell::Distribution::none:
      ui->randomize->setCheckState(Qt::CheckState::Unchecked);
      ui->startVal->setValue(param->val[0]);
      ui->incAmt->setValue(param->val[1]);
      break;
    case PvarsCell::Distribution::lognormal:
      ui->randomize->setCheckState(Qt::CheckState::Checked);
      ui->lognormalDist->setChecked(true);
      ui->mean->setValue(param->val[0]);
      ui->stdDev->setValue(param->val[1]);
      break;
    case PvarsCell::Distribution::normal:
      ui->randomize->setCheckState(Qt::CheckState::Checked);
      ui->normalDist->setChecked(true);
      ui->mean->setValue(param->val[0]);
      ui->stdDev->setValue(param->val[1]);
      break;
  }
}

void AddSingleCellPvar::updateIonChannelType() {
  QRegExp allowed_vars = QRegExp("Factor|Conc");
  ui->ionChannelType->clear();
  for (auto& pvarName : this->proto->cell()->pars()) {
    if (allowed_vars.indexIn(pvarName.c_str()) != -1) {
      ui->ionChannelType->addItem(pvarName.c_str());
    }
  }
  for (int i = 0; i < ui->ionChannelType->count(); ++i) {
    auto tip = this->pvarsDescriptions[ui->ionChannelType->itemText(i)];
    ui->ionChannelType->setItemData(i, tip, Qt::ToolTipRole);
  }
}

void AddSingleCellPvar::on_randomize_stateChanged(int state) {
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

void AddSingleCellPvar::on_normalDist_toggled(bool checked) {
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
void AddSingleCellPvar::on_addButton_clicked() {
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
  this->proto->pvars().insert(type, toAdd);
  emit pvarsChanged();
}

void AddSingleCellPvar::changeProto(shared_ptr<Protocol> proto) {
  this->proto = proto;
}

void AddSingleCellPvar::changeCell(shared_ptr<Cell>) {
  this->updateIonChannelType();
}
