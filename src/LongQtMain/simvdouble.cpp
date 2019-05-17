#include "simvdouble.h"
#include "oneitemlayout.h"

#include <protocol.h>
using namespace std;
using namespace LongQt;

SimvDouble::SimvDouble(std::shared_ptr<LQ::Protocol> proto, std::string name,
                       QDoubleSpinBox *parent)
    : Simvar(proto, name, parent) {
  parent->setToolTip(this->getToolTip());
  parent->setSuffix(" " + this->descriptions[name.c_str()]["Units"]);
  parent->setRange(/*std::numeric_limits<double>::min()*/ -100000,
                   std::numeric_limits<double>::max());

  connect(parent, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this,
          &SimvDouble::update_model);
}

void SimvDouble::update_ui() {
  auto parent = static_cast<QDoubleSpinBox *>(this->parent());
  string value = proto->parsStr(this->name);
  parent->setValue(std::stod(value));
  emit updated();
}

void SimvDouble::update_model(double value) {
  proto->parsStr(name, std::to_string(value));
}

SimvDouble::~SimvDouble() {}
