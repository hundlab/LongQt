#include "simvbool.h"
#include "oneitemlayout.h"

#include <protocol.h>
using namespace std;
using namespace LongQt;

SimvBool::SimvBool(shared_ptr<Protocol> proto, string name, QCheckBox *parent)
    : Simvar(proto, name, parent) {
  parent->setToolTip(this->getToolTip());
  connect(parent, &QCheckBox::stateChanged, this, &SimvBool::update_model);
}

void SimvBool::update_ui() {
  string value = proto->parsStr(this->name);
  static_cast<QCheckBox *>(parent())->setChecked(CellUtils::stob(value));
  emit updated();
}

void SimvBool::update_model(int value) {
  bool changed = !(((bool)value) == CellUtils::stob(proto->parsStr(name)));
  proto->parsStr(name, CellUtils::to_string(value));
  if (name == "secondStim" && changed) {
    throw std::abort;
  }
}

SimvBool::~SimvBool() {}
