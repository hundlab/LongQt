#include "simvcell.h"
#include "oneitemlayout.h"

#include <protocol.h>
using namespace std;
using namespace LongQt;

SimvCell::SimvCell(shared_ptr<Protocol> proto, string name, QComboBox* parent)
    : Simvar(proto, name, parent) {
  parent->setToolTip(this->getToolTip());
  QStringList cell_options;
  for (auto& im : proto->cellOptions()) {
    cell_options << im.c_str();
  }
  parent->addItems(cell_options);
  connect(parent,
          QOverload<const QString&>::of(&QComboBox::currentIndexChanged), this,
          &SimvCell::update_model);
}

void SimvCell::update_ui() {
  auto parent = static_cast<QComboBox*>(this->parent());
  int index = parent->findText(proto->parsStr(this->name).c_str());
  if (index != -1) {
    bool state = parent->blockSignals(true);
    parent->setCurrentIndex(index);
    parent->blockSignals(state);
  }
  emit updated();
}

void SimvCell::update_model(QString value) {
  proto->parsStr(name, value.toStdString());
  CellUtils::set_default_vals(*this->proto);
  emit cellChanged(this->proto->cell());
}

void SimvCell::changeProto(std::shared_ptr<Protocol> proto) {
  Simvar::changeProto(proto);
  auto parent = static_cast<QComboBox*>(this->parent());
  bool signalState = parent->blockSignals(true);
  parent->clear();
  QStringList cell_options;
  for (auto& im : proto->cellOptions()) {
    cell_options << im.c_str();
  }
  parent->addItems(cell_options);
  parent->blockSignals(signalState);
}

SimvCell::~SimvCell() {}
