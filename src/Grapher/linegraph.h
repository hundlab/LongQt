/*
 * responsible for creating line graphs
 */
#ifndef LINEGRAPH_H
#define LINEGRAPH_H

#include <QDir>
#include <QList>
#include <QMap>
#include <QPair>
#include <QShortcut>
#include <QVector>
#include <QWidget>
#include "datareader.h"

namespace Ui {
class LineGraph;
}

class LineGraph : public QWidget {
  Q_OBJECT
 public:
  explicit LineGraph(QString xLabel, QString yLabel, QDir saveDir,
                     QWidget* parent = 0);
  ~LineGraph();
  void addData(QVector<double>& x, QVector<double>& y, QString name);
  void populateList(LongQt::DataReader::SimData data);

 private:
  void Initialize();

  Ui::LineGraph* ui;
  QList<QVector<double>> y;
  QList<QVector<double>> x;
  QString xLabel;
  QString yLabel;
  QDir saveDir;
  QMap<QString, QMap<QString, QString>> descMap;
  int controlLocation;
  QShortcut* plus;
  QShortcut* minus;
  QShortcut* up;
  QShortcut* down;
  QShortcut* left;
  QShortcut* right;
 private slots:
  bool control_on_graph(QVector<double>& x, QVector<double>& y, QString& name);
  void on_save_clicked();
//  void on_loadControl_clicked();
  void on_chooseGraphs_clicked();
  void on_toggleLegend_clicked();
  void zoomIn();
  void zoomOut();
  void shiftLeft();
  void shiftRight();
  void shiftUp();
  void shiftDown();
};

#endif  // LINEGRAPH_H
