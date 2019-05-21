#include "voltageclampdelegate.h"

#include <QDoubleSpinBox>
#include <QPainter>

VoltageClampDelegate::VoltageClampDelegate(QWidget *parent)
    : QStyledItemDelegate(parent) {}

QWidget *VoltageClampDelegate::createEditor(
    QWidget *parent, const QStyleOptionViewItem & /*option*/,
    const QModelIndex &index) const {
  if (index.column() == 0) {
    auto widg = new QDoubleSpinBox(parent);
    widg->setFrame(false);
    widg->setRange(0, 1000000000);
    widg->setSuffix(" ms");
    return widg;
  } else if (index.column() == 1) {
    auto widg = new QDoubleSpinBox(parent);
    widg->setFrame(false);
    widg->setRange(-1000, 1000);
    widg->setSuffix(" mV");
    return widg;
  }
  return 0;
  //    connect(widg, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
  //    this, &VoltageClampSetupWidget::handleDoubleUpdate);
}

void VoltageClampDelegate::setEditorData(QWidget *editor,
                                         const QModelIndex &index) const {
  double value = index.model()->data(index, Qt::EditRole).toDouble();
  auto box = static_cast<QDoubleSpinBox *>(editor);
  box->setValue(value);
}

void VoltageClampDelegate::setModelData(QWidget *editor,
                                        QAbstractItemModel *model,
                                        const QModelIndex &index) const {
  auto box = static_cast<QDoubleSpinBox *>(editor);
  box->interpretText();
  double value = box->value();

  model->setData(index, value, Qt::EditRole);
}

void VoltageClampDelegate::updateEditorGeometry(
    QWidget *editor, const QStyleOptionViewItem &option,
    const QModelIndex &index) const {
  editor->setGeometry(option.rect);
}
