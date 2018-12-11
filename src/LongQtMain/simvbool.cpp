#include "simvbool.h"
#include "oneitemlayout.h"

#include <protocol.h>
using namespace std;
using namespace LongQt;

SimvBool::SimvBool(shared_ptr<Protocol> proto, string name, QWidget *parent)
    : Simvar(proto, name, parent) {
  this->widg = new QCheckBox();
  auto layout = new OneItemLayout(this);
  layout->addWidget(widg);
  connect(widg, &QCheckBox::stateChanged, this, &SimvBool::update_model);
}

void SimvBool::update_ui() {
  string value = proto->parsStr(this->name);
  widg->setChecked(CellUtils::stob(value));
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
