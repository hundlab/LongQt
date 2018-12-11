#ifndef LABELAXISTICKER_H
#define LABELAXISTICKER_H

#include <QString>
#include <QVector>
#include "qcustomplot.h"

class LabelAxisTicker : public QCPAxisTicker {
 public:
  LabelAxisTicker();
  void labels(QVector<QString>& labels);
  QVector<QString> labels();

 protected:
  virtual QString getTickLabel(double tick, const QLocale& locale,
                               QChar formatChar, int precision) override;
  virtual double getTickStep(const QCPRange& range) override;
  virtual int getSubTickCount(double tickStep) override;

 private:
  QVector<QString> __labels;
};
#endif
