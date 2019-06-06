#include "linegraph.h"
#include <QFileInfo>
#include "cellutils.h"
#include "choosegraphs.h"
#include "currentClampProtocol.h"
#include "guiUtils.h"
#include "ui_linegraph.h"

namespace LQ = LongQt;

LineGraph::LineGraph(QString xLabel, QString yLabel, QDir saveDir,
                     QWidget* parent)
    : QWidget(parent), ui(new Ui::LineGraph) {
  ui->setupUi(this);
  this->unitsMap = GuiUtils::readMap(":/hoverText/dvarsUnits.json");
  this->controlLocation = -1;
  this->xLabel = xLabel;
  this->yLabel = yLabel;
  this->saveDir = saveDir;
  this->Initialize();
}
void LineGraph::Initialize() {
  this->plus = new QShortcut(QKeySequence(Qt::Key_I), this);
  this->minus = new QShortcut(QKeySequence(Qt::Key_O), this);
  this->left = new QShortcut(QKeySequence(Qt::Key_Left), this);
  this->right = new QShortcut(QKeySequence(Qt::Key_Right), this);
  this->up = new QShortcut(QKeySequence(Qt::Key_Up), this);
  this->down = new QShortcut(QKeySequence(Qt::Key_Down), this);
  connect(plus, &QShortcut::activated, this, &LineGraph::zoomIn);
  connect(minus, &QShortcut::activated, this, &LineGraph::zoomOut);
  connect(left, &QShortcut::activated, this, &LineGraph::shiftLeft);
  connect(right, &QShortcut::activated, this, &LineGraph::shiftRight);
  connect(up, &QShortcut::activated, this, &LineGraph::shiftUp);
  connect(down, &QShortcut::activated, this, &LineGraph::shiftDown);
  ui->plot->setOpenGl(true);
  ui->plot->xAxis->grid()->setVisible(false);
  ui->plot->yAxis->grid()->setVisible(false);
  ui->plot->xAxis->setLabelFont(QFont(font().family(), 16));
  ui->plot->yAxis->setLabelFont(QFont(font().family(), 16));
  ui->plot->xAxis->setLabel("Time (ms)");
  ui->plot->yAxis->setLabel(this->yLabel + " (" + this->unitsMap[this->yLabel] +
                            ")");
  ui->plot->setInteractions(QCP::iRangeZoom | QCP::iRangeDrag);
  ui->plot->axisRect()->setRangeZoom(ui->plot->xAxis->orientation());
  ui->plot->axisRect()->setRangeDrag(ui->plot->xAxis->orientation());
  ui->plot->legend->setVisible(true);

  if (ui->plot->plotLayout()->rowCount() < 2) {
    ui->plot->plotLayout()->insertRow(0);
  }
  if (ui->plot->plotLayout()->elementAt(0) != 0) {
    ui->plot->plotLayout()->removeAt(0);
  }
  ui->plot->plotLayout()->addElement(
      0, 0, new QCPTextElement(ui->plot, this->yLabel + " vs " + this->xLabel));
}

LineGraph::~LineGraph() { delete ui; }

void LineGraph::addData(QVector<double>& x, QVector<double>& y, QString name) {
  if (x.isEmpty() || y.isEmpty()) {
    return;
  }
  this->x.append(x);
  this->y.append(y);
  ui->plot->addGraph();
  ui->plot->graph()->setData(x, y, true);
  ui->plot->graph()->setPen(
      QPen(QBrush(GuiUtils::genMutedColor(this->x.size())), 3));
  ui->plot->graph()->setName(name);
  ui->plot->yAxis->rescale();
  ui->plot->xAxis->setRange(x.first(), x.last());
  ui->plot->replot();
}
void LineGraph::on_save_clicked() {
  ui->plot->saveJpg(
      saveDir.absolutePath() + "/" + yLabel + "vs" + xLabel + ".jpg", 0, 0, 1.0,
      -1);
}
void LineGraph::on_loadControl_clicked() {
  QVector<double> x, y;
  QString name;

  if (control_on_graph(x, y, name)) {
    if (controlLocation >= 0 && controlLocation < x.size()) {
      if (ui->plot->graph(controlLocation) != 0) {
        ui->plot->removeGraph(controlLocation);
      }
      this->x.removeAt(controlLocation);
      this->y.removeAt(controlLocation);
    }
    this->addData(x, y, name);
    controlLocation = this->x.size() - 1;
  }
}
bool LineGraph::control_on_graph(QVector<double>& x, QVector<double>& y,
                                 QString& name) {
  QString location = QFileDialog::getExistingDirectory(
      Q_NULLPTR, "Choose Data Directory",
      QStandardPaths::standardLocations(QStandardPaths::DocumentsLocation)
          .first());
  if (location == "") {
    return false;
  }

  return false;
}
void LineGraph::populateList(LQ::DataReader::SimData data) {
  for (int trial = 0; trial < data.meas.size(); ++trial) {
    auto& meas = data.meas[trial];
    for (int pos = 0; pos < meas.header.size(); ++pos) {
      QString var_name = meas.header[pos].var_name.c_str();
      QString prop_name = meas.header[pos].prop_name.c_str();
      if (var_name != yLabel || meas.data[pos].size() == 0) continue;
      QString name_info =
          GuiUtils::getName(trial, meas.header[pos].cell_info_parsed);
      new QTreeWidgetItem(
          ui->treeWidget,
          {name_info, prop_name, QString::number(meas.data[pos].back())});
    }
  }
  for (int i = 0; i < ui->treeWidget->columnCount(); ++i) {
    ui->treeWidget->resizeColumnToContents(i);
  }
  //    string filename =
  //    CellUtils::strprintf(CurrentClamp().finalpropertyoutfile.c_str(),
  //    trial);
  ////    filename = CellUtils::strprintf(filename.c_str(),
  //    QFile file(saveDir.absolutePath() +"/"+ QString(filename.c_str()));
  //    if(!file.open(QIODevice::ReadOnly)){
  //        return;
  //    }
  //    QTextStream in(&file);
  //    QStringList names = in.readLine().trimmed().split("\t");
  //    QStringList values = in.readLine().trimmed().split("\t");
  //    auto it = names.begin();
  //    auto iv = values.begin();
  //    for(;it != names.end()&& iv != values.end(); it++,iv++) {
  //        QStringList nameParts = it->split("/");
  //        if(nameParts[0] == yLabel
  //            && nameParts.size()>1) {
  //            new QTreeWidgetItem(ui->treeWidget,{nameParts[1],*iv});
  //        }
  //    }
}
void LineGraph::on_chooseGraphs_clicked() {
  ChooseGraphs* choose = new ChooseGraphs(ui->plot, this);
  choose->setWindowTitle(this->yLabel);
  choose->exec();
}
void LineGraph::on_toggleLegend_clicked() {
  ui->plot->legend->setVisible(!ui->plot->legend->visible());
  ui->plot->replot();
}

QCPRange zoom(QCPRange range, double scale) {
  double diff = scale * (range.center() - range.lower);
  return QCPRange(range.center() + diff, range.center() - diff);
}

void LineGraph::zoomIn() {
  this->ui->plot->yAxis->setRange(zoom(this->ui->plot->yAxis->range(), .75));
  this->ui->plot->xAxis->setRange(zoom(this->ui->plot->xAxis->range(), .75));
  this->ui->plot->replot();
}

void LineGraph::zoomOut() {
  double zoomOutScale = 1.0 / 0.75;
  this->ui->plot->yAxis->setRange(
      zoom(this->ui->plot->yAxis->range(), zoomOutScale));
  this->ui->plot->xAxis->setRange(
      zoom(this->ui->plot->xAxis->range(), zoomOutScale));
  this->ui->plot->replot();
}

void LineGraph::shiftLeft() {
  this->ui->plot->xAxis->setRange(
      this->ui->plot->xAxis->range() -
      (this->ui->plot->xAxis->range().size() * .05));
  this->ui->plot->replot();
}

void LineGraph::shiftRight() {
  this->ui->plot->xAxis->setRange(
      this->ui->plot->xAxis->range() +
      (this->ui->plot->xAxis->range().size() * .05));
  this->ui->plot->replot();
}

void LineGraph::shiftUp() {
  this->ui->plot->yAxis->setRange(
      this->ui->plot->yAxis->range() +
      (this->ui->plot->yAxis->range().size() * .05));
  this->ui->plot->replot();
}

void LineGraph::shiftDown() {
  this->ui->plot->yAxis->setRange(
      this->ui->plot->yAxis->range() -
      (this->ui->plot->yAxis->range().size() * .05));
  this->ui->plot->replot();
}
