#ifndef ORDINALSPINBOX_H
#define ORDINALSPINBOX_H

#include <QSpinBox>

class OrdinalSpinBox : public QSpinBox {
 public:
  OrdinalSpinBox(QWidget* parent = nullptr);

 protected:
  virtual QString textFromValue(int val) const override;
  virtual int valueFromText(const QString& text) const override;
};

#endif  // ORDINALSPINBOX_H
