#include "checkandwarn.h"
#include <QListWidget>
#include "guiUtils.h"
#include "ui_checkandwarn.h"

CheckAndWarn::CheckAndWarn(std::shared_ptr<LongQt::Protocol> proto,
                           QWidget* parent)
    : QDialog(parent), ui(new Ui::CheckAndWarn) {
  ui->setupUi(this);
  this->simvDesc = GuiUtils::readDesc(":/hoverText/simvarsDesc.json");
  this->measDesc = GuiUtils::readDesc(":/hoverText/measDesc.json");
  this->cellParsDesc = GuiUtils::readDesc(":/hoverText/cellParsDescDesc.json");
  this->cellVarsDesc = GuiUtils::readDesc(":/hoverText/cellVarsDesc.json");
  if (!runChecks(proto)) {
    ui->warningsList->addItems(this->warningsList);
  }
  connect(ui->cancelButton, &QPushButton::pressed, this, &CheckAndWarn::reject);
  connect(ui->acceptButton, &QPushButton::pressed, this, &CheckAndWarn::accept);
}

bool CheckAndWarn::hasWarnings() { return this->warningsList.size() != 0; }

bool CheckAndWarn::runChecks(std::shared_ptr<LongQt::Protocol> proto) {
  bool checkSucceeded = true;
  checkSucceeded &= checkWritetimeTooLarge(proto);
  checkSucceeded &= checkMeasTimeTooLarge(proto);
  // add additional checks here
  return checkSucceeded;
}

bool CheckAndWarn::checkWritetimeTooLarge(
    std::shared_ptr<LongQt::Protocol> proto) {
  if (!proto->hasPar("writetime")) {
    return true;
  }
  double writetime = std::stod(proto->parsStr("writetime"));
  double tMax = std::stod(proto->parsStr("tMax"));
  if (writetime > tMax) {
    QString writetimeName = this->simvDesc["writetime"]["Name"];
    QString tMaxName = this->simvDesc["tMax"]["Name"];
    this->warningsList.append(writetimeName + " is greater than " + tMaxName +
                              ". No traces will be written.");
    return false;
  }
  return true;
}

bool CheckAndWarn::checkMeasTimeTooLarge(
    std::shared_ptr<LongQt::Protocol> proto) {
  if (!proto->hasPar("meastime")) {
    return true;
  }
  double meastime = std::stod(proto->parsStr("meastime"));
  double tMax = std::stod(proto->parsStr("tMax"));
  int numSelected = proto->measureMgr().selection().size();
  if (meastime > tMax && numSelected > 0) {
    QString meastimeName = this->simvDesc["meastime"]["Name"];
    QString tMaxName = this->simvDesc["tMax"]["Name"];
    this->warningsList.append(meastimeName + " is greater than " + tMaxName +
                              ". No variables will be measured.");
    return false;
  }
  return true;
}
