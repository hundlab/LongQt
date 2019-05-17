#include "oneitemlayout.h"

OneItemLayout::OneItemLayout() : QLayout() {}

OneItemLayout::OneItemLayout(QWidget *parent) : QLayout(parent) {}

OneItemLayout::~OneItemLayout() {}

void OneItemLayout::setGeometry(const QRect &r) {
  item ? item->setGeometry(r) : (void)0;
}

QSize OneItemLayout::sizeHint() const {
  return (item ? item->sizeHint() : QSize());
}

QSize OneItemLayout::minimumSize() const {
  return (item ? item->minimumSize() : QSize());
}

bool OneItemLayout::hasHeightForWidth() const {
  return (item ? item->hasHeightForWidth() : false);
}

int OneItemLayout::heightForWidth(int w) const {
  return (item ? item->heightForWidth(w) : 0);
}

void OneItemLayout::addItem(QLayoutItem *item) {
  if (this->item) this->item->widget()->deleteLater();
  this->item.reset(item);
}

QLayoutItem *OneItemLayout::itemAt(int index) const {
  return (index == 0 ? item.data() : nullptr);
}

QLayoutItem *OneItemLayout::takeAt(int index) {
  return (index == 0 ? item.take() : nullptr);
}

int OneItemLayout::count() const { return (item ? 1 : 0); }
