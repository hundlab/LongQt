#include "simvcell.h"
#include "oneitemlayout.h"

#include <protocol.h>
using namespace std;
using namespace LongQt;

SimvCell::SimvCell(shared_ptr<Protocol> proto, string name, QWidget* parent)
    : Simvar(proto, name, parent) {
  this->widg = new QComboBox();
  auto layout = new OneItemLayout(this);
  layout->addWidget(widg);
  QStringList cell_options;
  for (auto& im : proto->cellOptions()) {
    cell_options << im.c_str();
  }
  widg->addItems(cell_options);
  connect(widg, QOverload<const QString&>::of(&QComboBox::currentIndexChanged),
          this, &SimvCell::update_model);
}

void SimvCell::update_ui() {
  int index = widg->findText(proto->parsStr(this->name).c_str());
  if (index != -1) {
    bool state = widg->blockSignals(true);
    widg->setCurrentIndex(index);
    widg->blockSignals(state);
  }
  emit updated();
}

void SimvCell::update_model(QString value) {
  proto->parsStr(name, value.toStdString());
  CellUtils::set_default_vals(*this->proto);
  emit cellChanged(this->proto->cell());
}

SimvCell::~SimvCell() {}
