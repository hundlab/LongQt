#include "labelaxisticker.h"

LabelAxisTicker::LabelAxisTicker() : QCPAxisTicker() {
  this->setTickStepStrategy(QCPAxisTicker::tssMeetTickCount);
  this->setTickOrigin(1);
}

void LabelAxisTicker::labels(QVector<QString>& labels) {
  this->__labels = labels;
  this->setTickCount(__labels.length());
}
QVector<QString> LabelAxisTicker::labels() { return this->__labels; }
QString LabelAxisTicker::getTickLabel(double tick, const QLocale&, QChar, int) {
  return this->__labels.value((int)tick - 1);
}

int LabelAxisTicker::getSubTickCount(double) { return 0; }

double LabelAxisTicker::getTickStep(const QCPRange&) { return 1; }
