#include "graph.h"
#include "bargraph.h"
#include "datareader.h"
#include "guiUtils.h"
#include "linegraph.h"
#include "loadingprogress.h"
#include "measure.h"
#include "ui_graph.h"

#include <QGridLayout>
#include <QIODevice>
#include <QList>
#include <QMap>
#include <QPointer>
#include <QProgressDialog>
#include <QScopedPointer>
#include <algorithm>
#include <exception>

namespace LQ = LongQt;

Grapher::Grapher(QDir read_location, QWidget* parent)
    : QDialog(parent), ui(new Ui::Grapher) {
  ui->setupUi(this);
  this->read_location = read_location;
  this->Initialize();
}
void Grapher::Initialize() {
  std::string read_location_name = read_location.absolutePath().toStdString();
  auto trialNums = LQ::DataReader::getTrialNums(read_location_name);
  std::set<int> excludes;
  QSet<int> qtrialNums;
  for (auto trialNum : trialNums) {
    qtrialNums.insert(trialNum);
  }
  QScopedPointer<LoadingProgressDialog> loadingOptions(
      new LoadingProgressDialog(qtrialNums, this));
  if (trialNums.size() > 10) {
    if (QDialog::Accepted == loadingOptions->exec()) {
      auto excludesList = loadingOptions->getExcludes();
      excludes.insert(excludesList.begin(), excludesList.end());
    }
  }
  auto data = LQ::DataReader::readDir(read_location_name, excludes);

  if (data.trace.size() == 0 && data.meas.size() == 0) {
    QMessageBox::warning(0, "Error", "No Files Found");
    throw std::runtime_error("No Files Found");
  }
  numSims = 1;
  this->buildLineGraphs(data);
  this->buildBarGraphs(data);
  this->setWindowTitle(this->read_location.dirName());
}

Grapher::~Grapher() { delete ui; }

void Grapher::buildLineGraphs(LQ::DataReader::SimData data, QString prefix) {
  QString xName = "t";

  //  int progressCounter = 0;
  //  QScopedPointer<LoadingProgressDialog> loadingOptions(
  //      new LoadingProgressDialog(files, this));
  //  if (files.length() > 10) {
  //    if (QDialog::Accepted == loadingOptions->exec()) {
  //      files = loadingOptions->getFilesToLoad();
  //    } else {
  //      files.clear();
  //    }
  //  }
  //  QScopedPointer<QProgressDialog> progressDisp(
  //      new QProgressDialog("Opening Files", "Skip", 0, files.size(), this));
  //  progressDisp->setValue(0);
  //  progressDisp->show();

  for (int trial = 0; trial < data.trace.size(); ++trial) {
    auto& trace = data.trace[trial];
    auto& meas = data.meas[trial];
    if (meas.header.size() == 0 && trace.header.size() == 0) {
      continue;
    }
    //    progressDisp->setValue(progressCounter);
    //    QCoreApplication::processEvents();
    //    if (progressDisp->wasCanceled()) {
    //      break;
    //    }

    QMap<QString, int> xIndex;
    for (int pos = 0; pos < trace.data.size(); ++pos) {
      if (trace.header[pos].var_name.c_str() == xName) {
        xIndex[trace.header[pos].cell_info.c_str()] = pos;
      }
    }
    if (xIndex.isEmpty()) {
      QMessageBox::warning(
          0, "ERROR NO TIME",
          "No variable for " + xName + " which is the selected x variable");
      return;
    }

    for (int pos = 0; pos < trace.data.size(); ++pos) {
      auto cell_info = QString(trace.header[pos].cell_info.c_str());
      int xpos = xIndex[cell_info];
      if (xpos == pos) {
        continue;
      }
      auto var_name = QString(trace.header[pos].var_name.c_str());
      auto xdata = QVector<double>::fromStdVector(trace.data[xpos]);
      auto ydata = QVector<double>::fromStdVector(trace.data[pos]);
      if (!graphMap.contains(var_name)) {
        auto graph = new LineGraph(xName, var_name, this->read_location);
        graphMap.insert(var_name, graph);
        ui->tabWidget->addTab(graph, var_name);
        graph->populateList(data);
      }
      graphMap[var_name]->addData(
          xdata, ydata,
          GuiUtils::getName(trial, trace.header[pos].cell_info_parsed, prefix));
    }
    //    progressCounter++;
  }
}

void Grapher::buildBarGraphs(LongQt::DataReader::SimData data, QString prefix) {
  if (data.meas.size() == 0) return;
  if (data.meas.size() < 2 && data.meas[0].header.size() < 2) return;
  for (int trial = 0; trial < data.meas.size(); ++trial) {
    auto& meas = data.meas[trial];
    for (int pos = 0; pos < meas.data.size(); ++pos) {
      QString instance =
          GuiUtils::getName(trial, meas.header[pos].cell_info_parsed, prefix);
      QString var = meas.header[pos].var_name.c_str();
      QString property = meas.header[pos].prop_name.c_str();
      if (!barMap.contains(var + ":" + property)) {
        auto bar =
            new barGraph(instance, meas.data[pos].back(), var, read_location);
        ui->tabWidget->addTab(bar, var + ":" + property);
        barMap.insert(var + ":" + property, bar);
      } else {
        barMap[var + ":" + property]->addBar(instance, meas.data[pos].back());
      }
    }
  }
}

void Grapher::on_loadNew_clicked() {
  QString location = QFileDialog::getExistingDirectory(
      Q_NULLPTR, "Choose Data Directory",
      QStandardPaths::standardLocations(QStandardPaths::DocumentsLocation)
          .first());
  if (location == "") {
    return;
  }
  numSims++;
  auto data = LQ::DataReader::readDir(location.toStdString());
  this->buildLineGraphs(data, "Sim " + QString::number(numSims) + " ");
  this->buildBarGraphs(data, "Sim " + QString::number(numSims) + " ");
}
