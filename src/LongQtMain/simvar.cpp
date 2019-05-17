#include "simvar.h"
#include "guiUtils.h"
#include "oneitemlayout.h"

using namespace std;
using namespace LongQt;

QMap<QString, QMap<QString, QString>> Simvar::descriptions;

Simvar::Simvar(shared_ptr<Protocol> proto, string name, QWidget *parent)
    : QObject(parent) {
  this->name = name;
  this->proto = proto;
  if (this->descriptions.isEmpty()) {
    this->descriptions = GuiUtils::readDesc(":/hoverText/simvarsDesc.json");
  }
}

void Simvar::changeProto(shared_ptr<Protocol> proto) { this->proto = proto; }

void Simvar::changeCell(shared_ptr<Cell>) { this->update_ui(); }

Simvar::~Simvar() {}

void Simvar::setupLabel(QLabel *label) {
  label->setText(this->getPrettyName());
  label->setToolTip(this->getToolTip());
}

QString Simvar::getPrettyName() {
  return this->descriptions[this->name.c_str()]["Name"];
}

QString Simvar::getToolTip() {
  return this->descriptions[this->name.c_str()]["Description"];
}
