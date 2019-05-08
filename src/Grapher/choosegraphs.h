/*
 * A dialog to allow users to toggle the displaying of graphs
 */
#ifndef CHOOSEGRAPHS_H
#define CHOOSEGRAPHS_H

#include <QDialog>
#include "qcustomplot.h"

namespace Ui {
class ChooseGraphs;
}

class ChooseGraphs : public QDialog {
  Q_OBJECT

 public:
  explicit ChooseGraphs(QCustomPlot* plot, QWidget* parent = 0);
  ~ChooseGraphs();

 public slots:
  void updateNames();

 signals:
  void namesChanged();

 private:
  Ui::ChooseGraphs* ui;
  QCustomPlot* plot;
};

#endif  // CHOOSEGRAPHS_H
