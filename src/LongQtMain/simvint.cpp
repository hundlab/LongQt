#include "simvint.h"
#include "oneitemlayout.h"

#include <protocol.h>
#include <QGridLayout>
using namespace std;
using namespace LongQt;

SimvInt::SimvInt(shared_ptr<Protocol> proto, string name, QSpinBox *parent)
    : Simvar(proto, name, parent) {
  parent->setToolTip(this->getToolTip());
  parent->setSuffix(" " + this->descriptions[name.c_str()]["Units"]);
  parent->setRange(/*std::numeric_limits<double>::min()*/ -100000,
                   std::numeric_limits<int>::max());

  connect(parent, QOverload<int>::of(&QSpinBox::valueChanged), this,
          &SimvInt::update_model);
}

void SimvInt::update_ui() {
  auto parent = static_cast<QSpinBox *>(this->parent());
  string value = proto->parsStr(this->name);
  parent->setValue(std::stoi(value));
  emit updated();
}

void SimvInt::update_model(int value) {
  proto->parsStr(name, to_string(value));
}

SimvInt::~SimvInt() {}
