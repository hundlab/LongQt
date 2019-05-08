#include "ordinalspinbox.h"

OrdinalSpinBox::OrdinalSpinBox(QWidget *parent) : QSpinBox(parent) {}

QString OrdinalSpinBox::textFromValue(int val) const {
  QString text = QString::number(val);
  int last = val % 10;
  int tens = (int(val/10) % 10);
  if (tens == 1) {
    return text + "th";
  }
  switch (last) {
    case 1:
      return text + "st";
      break;
    case 2:
      return text + "nd";
      break;
    case 3:
      return text + "rd";
      break;

    default:
      return text + "th";
      break;
  }
}

int OrdinalSpinBox::valueFromText(const QString &text) const {
  return std::stoi(text.toStdString());
}
