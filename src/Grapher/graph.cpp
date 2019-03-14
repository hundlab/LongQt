#include "graph.h"
#include "bargraph.h"
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

Grapher::Grapher(QDir read_location, QWidget* parent)
    : QDialog(parent), ui(new Ui::Grapher) {
  ui->setupUi(this);
  this->read_location = read_location;
  this->Initialize();
}
void Grapher::Initialize() {
  auto dssData = this->dssData();
  auto fileNames = this->getFileNames();
  this->buildLineGraphs(fileNames, dssData);
  this->buildBarGraphs(dssData);
  this->setWindowTitle(this->read_location.dirName());
}
QFileInfoList Grapher::getFileNames() {
  return this->getFileNames(this->read_location);
}
QFileInfoList Grapher::getFileNames(QDir location) {
  QFileInfoList toReturn;
  for (QFileInfo file : location.entryInfoList()) {
    if (QRegExp("trace\\d+").exactMatch(file.baseName())) {
      toReturn << file;
    }
  }
  return toReturn;
}
QFileInfoList Grapher::getFileNamesBar(QDir location) {
  QFileInfoList toReturn;
  for (QFileInfo file : location.entryInfoList(QDir::Files)) {
    if (QRegExp("meas\\d+").exactMatch(file.baseName())) {
      toReturn << file;
    }
  }
  return toReturn;
}
QString Grapher::getName(QFileInfo file, QString position) {
  QStringList trial =
      file.baseName().split(QRegExp("\\D+"), QString::SkipEmptyParts);
  QStringList nums = position.split(QRegExp("\\D+"), QString::SkipEmptyParts);
  if (nums.length() > 1) {
    return "Cell " + nums[0] + " " + nums[1];
  } else if (trial.length() > 0) {
    return "Trial " + trial[0];
  } else {
    return "Cell";
  }
}

Grapher::~Grapher() { delete ui; }

void Grapher::buildLineGraphs(QFileInfoList files, DssD dssData) {
  QString xName = "t";
  if (files.isEmpty()) {
    QMessageBox::warning(0, "Error", "No Files Found");
    throw std::runtime_error("No Files Found");
  }
  QMap<QString, LineGraph*> graphMap;
  int progressCounter = 0;
  QScopedPointer<LoadingProgressDialog> loadingOptions(
      new LoadingProgressDialog(files, this));
  if (files.length() > 10) {
    if (QDialog::Accepted == loadingOptions->exec()) {
      files = loadingOptions->getFilesToLoad();
    } else {
      files.clear();
    }
  }
  QScopedPointer<QProgressDialog> progressDisp(
      new QProgressDialog("Opening Files", "Skip", 0, files.size(), this));
  progressDisp->setValue(0);
  progressDisp->show();

  for (QFileInfo fileInfo : files) {
    progressDisp->setValue(progressCounter);
    QCoreApplication::processEvents();
    if (progressDisp->wasCanceled()) {
      break;
    }
    QFile file(fileInfo.absoluteFilePath());
    if (!file.open(QIODevice::ReadOnly)) {
      QMessageBox::warning(
          0, "error", "File " + fileInfo.fileName() + " Could not be Opened");
      continue;
    }
    QTextStream in(&file);
    QVector<QVector<double>> y;
    QMap<QString, int> xIndex;
    QStringList header;
    // watch problems with first line of labels
    header = in.readLine().split("\t", QString::SkipEmptyParts);
    QStringList cells;
    QStringList props;
    for (auto name : header) {
      auto split = name.split("/");
      if (split.size() > 1) {
        cells.push_back(split[0]);
        props.push_back(split[1]);
      } else if (split.size() == 1) {
        cells.push_back("");
        props.push_back(split.first());
      }
      if (split.size() > 0) {
        if (props.last() == xName) {
          xIndex.insert(cells.last(), props.size() - 1);
        }
      }
    }
    if (xIndex.isEmpty()) {
      QMessageBox::warning(0, "ERROR NO TIME",
                           "File " + fileInfo.fileName() +
                               " had no variable for " + xName +
                               " which is the selected x variable");
      return;
    }

    y = QVector<QVector<double>>(header.length());
    while (!in.atEnd()) {
      QStringList values = in.readLine().split("\t", QString::SkipEmptyParts);
      for (int i = 0; i < values.length() && i < header.length(); i++) {
        y[i].push_back(values[i].toDouble());
      }
    }
    file.close();

    for (int i = 0; i < props.length(); i++) {
      int pos = xIndex[cells[i]];
      if (pos == i) {
        continue;
      }
      if (!graphMap.contains(props[i])) {
        auto graph = new LineGraph(props[pos], props[i], this->read_location);
        graphMap.insert(props[i], graph);
        ui->tabWidget->addTab(graph, props[i]);
        graph->populateList(dssData);
      }
      graphMap[props[i]]->addData(y[pos], y[i],
                                  this->getName(fileInfo, cells[i]));
    }
    progressCounter++;
  }
}
Grapher::DssD Grapher::dssData() {
  QFileInfoList fileInfos = this->getFileNamesBar(this->read_location);
  DssD dssDat;

  for (auto fileInfo : fileInfos) {
    QFile file(fileInfo.absoluteFilePath());
    if (!file.open(QIODevice::ReadOnly)) {
      continue;
    }
    QTextStream in(&file);
    QStringList header = in.readLine().split("\t", QString::SkipEmptyParts);
    QString line;
    while (!in.atEnd()) {
      line = in.readLine();
    }
    QStringList values = line.split("\t", QString::SkipEmptyParts);
    auto name = header.begin();
    auto value = values.begin();
    for (; name != header.end() && value != values.end(); name++, value++) {
      auto splitName = name->split("/");
      if (splitName.size() < 2) continue;
      QString property = splitName.last();
      QString var = splitName.size() == 3 ? splitName[1] : splitName.first();
      QString position = splitName.size() == 3 ? splitName.first() : "";
      auto instance = this->getName(fileInfo, position);
      dssDat.append(
          std::make_tuple(instance, var, property, value->toDouble()));
    }
  }
  return dssDat;
}
void Grapher::buildBarGraphs(DssD dssData) {
  QMap<QString, barGraph*> bars;
  for (auto value : dssData) {
    auto instance = std::get<0>(value);
    auto var = std::get<1>(value);
    auto property = std::get<2>(value);
    if (!bars.contains(var + ":" + property)) {
      auto bar = new barGraph(instance, std::get<3>(value), var, read_location);
      ui->tabWidget->addTab(bar, var + ":" + property);
      bars.insert(var + ":" + property, bar);
    } else {
      bars[var + ":" + property]->addBar(instance, std::get<3>(value));
    }
  }
}

void Grapher::on_loadNew_clicked() {
  QFileDialog getDirOrFile(0, tr("Open File"));
  getDirOrFile.setFileMode(QFileDialog::Directory);
  QDir loc = this->read_location;
  loc.cdUp();
  getDirOrFile.setDirectory(loc);
  getDirOrFile.setNameFilter(
      "Directories");  //;; All Files (*.*);;Data File (*.txt *.dat)");
  QStringList filenames;
  if (getDirOrFile.exec()) {
    filenames = getDirOrFile.selectedFiles();
  }
  if (filenames.isEmpty()) return;
  while (ui->tabWidget->count() > 0) {
    ui->tabWidget->removeTab(0);
  }
  QFileInfoList fileInfos;
  for (auto filename : filenames) {
    QFileInfo file(filename);
    this->setWindowTitle(file.fileName());
    if (file.isDir()) {
      fileInfos.append(this->getFileNames(QDir(file.absoluteFilePath())));
    } else {
      fileInfos.append(file);
    }
  }
  try {
    buildLineGraphs(fileInfos);
  } catch (std::runtime_error& e) {
    QMessageBox::warning(0, tr("Error"),
                         tr("Selected files could not be used"));
  }
}
