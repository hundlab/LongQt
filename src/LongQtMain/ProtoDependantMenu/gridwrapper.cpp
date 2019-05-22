#include "gridwrapper.h"
#include "gridProtocol.h"
#include "oneitemlayout.h"

GridWrapper::GridWrapper(std::shared_ptr<LongQt::Protocol> initial_proto,
                         QWidget *parent)
    : QWidget(parent) {
  this->changeProto(initial_proto);
  new OneItemLayout(this);
}

void GridWrapper::changeProto(std::shared_ptr<LongQt::Protocol> proto) {
  if (proto->type() == LQ::GridProtocol::name) {
    this->grid =
        new GridSetupWidget(std::static_pointer_cast<LQ::GridProtocol>(proto));
    connect(grid, &GridSetupWidget::cellChanged, this,
            &GridWrapper::cellChanged);
    this->layout()->addWidget(grid);
    emit widgetSet(true);
  } else {
    grid.clear();
    emit widgetSet(false);
  }
}
