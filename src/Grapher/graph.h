/*
 * the master grapher widget responsible for creating all necessary graphs
 */
#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QDir>
#include <QListWidget>
#include <exception>

#include "bargraph.h"
#include "datareader.h"
#include "linegraph.h"
#include "qcustomplot.h"

namespace Ui {
class Grapher;
}

class Grapher : public QDialog {
  Q_OBJECT
  typedef QVector<std::tuple<QString, QString, QString, double>> DssD;

 public:
  // a directory is specified instead of files so that all
  // the files can be loaded for ex in the case of multiple
  // trials
  explicit Grapher(QDir read_locaiton, QWidget *parent = 0);
  ~Grapher();

 private:
  void Initialize();
  QDir read_location;
  QMap<QString, LineGraph *> graphMap;
  QMap<QString, barGraph *> barMap;
  int numSims;
 private slots:
  void buildLineGraphs(LongQt::DataReader::SimData data, QString prefix = "");
  void buildBarGraphs(LongQt::DataReader::SimData data, QString prefix = "");
  void on_loadNew_clicked();

 private:
  Ui::Grapher *ui;
};

#endif  // DIALOG_H
