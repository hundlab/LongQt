/*
 * the master grapher widget responsible for creating all necessary graphs
 */
#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QDir>
#include <QListWidget>
#include <exception>

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
  QFileInfoList getFileNames(QDir location);
  QFileInfoList getFileNames();
  QFileInfoList getFileNamesBar(QDir location);
  QString getName(QFileInfo file, QString position);
  QDir read_location;
 private slots:
  void buildLineGraphs(QFileInfoList files, DssD dssData = {});
  void buildBarGraphs(DssD dssData);
  DssD dssData();
  void on_loadNew_clicked();

 private:
  Ui::Grapher *ui;
};

#endif  // DIALOG_H
