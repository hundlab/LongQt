#include "linegraph.h"
#include "ui_linegraph.h"
#include "choosegraphs.h"
#include "protocol.h"

lineGraph::lineGraph(QString xLabel, QString yLabel, QDir saveDir, QWidget* parent) :
    QWidget(parent),
    ui(new Ui::lineGraph)
{
    ui->setupUi(this);
    this->controlLocation = -1;
    this->xLabel = xLabel;
    this->yLabel = yLabel;
    this->saveDir = saveDir;
    this->Initialize();
}
void lineGraph::Initialize() {
//should not just be 0!!
     populateList(0);
     ui->plot->xAxis->grid()->setVisible(false);
     ui->plot->yAxis->grid()->setVisible(false);
     ui->plot->xAxis->setLabelFont(QFont(font().family(), 16));
     ui->plot->yAxis->setLabelFont(QFont(font().family(), 16));
     ui->plot->xAxis->setLabel("Time (ms)");
     ui->plot->yAxis->setLabel(this->yLabel);
     ui->plot->setInteractions(QCP::iRangeZoom | QCP::iRangeDrag);
     ui->plot->axisRect()->setRangeZoom(ui->plot->xAxis->orientation());
     ui->plot->axisRect()->setRangeDrag(ui->plot->xAxis->orientation());
     ui->plot->legend->setVisible(true);

     if(ui->plot->plotLayout()->rowCount() < 2){
        ui->plot->plotLayout()->insertRow(0);
     }
     if(ui->plot->plotLayout()->elementAt(0) != 0) {
        ui->plot->plotLayout()->removeAt(0);
     }
     ui->plot->plotLayout()->addElement(0,0, new QCPPlotTitle(ui->plot, this->yLabel +" vs "+ this->xLabel));
}

lineGraph::~lineGraph()
{
    delete ui;
}
QColor genColor(int num) {
    return QColor::fromHsv((num*4*17)%360,200,200);
}
void lineGraph::addData(QVector<double>& x, QVector<double>& y, QString name) {
        this->x.append(x);
        this->y.append(y);
        ui->plot->addGraph();
        ui->plot->graph()->setData(x, y);
        ui->plot->graph()->setPen(QPen(genColor(this->x.size())));
        ui->plot->graph()->setName(name);
        ui->plot->yAxis->rescale();
        ui->plot->xAxis->setRange(x.first(), x.last());
        ui->plot->replot();
}
void lineGraph::on_save_clicked() {
    ui->plot->saveJpg(saveDir.absolutePath() + "/" + yLabel + "vs" + xLabel + ".jpg", 0,0,1.0, -1);
}
void lineGraph::on_loadControl_clicked() {
    QVector<double> x, y;

    if (control_on_graph(x, y)){
        if(controlLocation >= 0 && controlLocation < x.size()) {
            if(ui->plot->graph(controlLocation) != 0) {
                ui->plot->removeGraph(controlLocation);
            }
            this->x.removeAt(controlLocation);
            this->y.removeAt(controlLocation);
        }
        this->addData(x,y,"Control");
        controlLocation = this->x.size()-1;
    }
}
bool lineGraph::control_on_graph(QVector<double> &x, QVector<double> &y){
    QString filename = QFileDialog::getOpenFileName(
                this,
                tr("Open File"),
                saveDir.absolutePath(),
                "All Files (*.*);;Text File(*.txt)"
                );
    QFile file1(filename);
    if(!file1.open(QIODevice::ReadOnly)){
        QMessageBox::information(0,"error", file1.errorString());
    }else{
    QTextStream in(&file1);
    int point = 0;
    QString fline = in.readLine();
    QStringList split_line = fline.split("\t");
    int varpos = split_line.indexOf(yLabel);
    if(varpos == -1){ QMessageBox::information(0,"Error", yLabel +" not found!"); return false;}
    int timepos = split_line.indexOf(xLabel);
    if(timepos == -1){ QMessageBox::information(0,"Error", xLabel +" not found!"); return false;}
    while(!in.atEnd()){
        QString line = in.readLine();
        QStringList split_line = line.split("\t");
        y.push_back(split_line[varpos].toDouble());
        x.push_back(split_line[timepos].toDouble());
        point++;
    }
    file1.close();
    return true;
    }
    return false;
}
void lineGraph::populateList(int trial) {
    char filename[1500];
    sprintf(filename, Protocol().finalpropertyoutfile.c_str(), trial, yLabel.toStdString().c_str());
    QFile file(saveDir.absolutePath() +"/"+ QString(filename));
    if(!file.open(QIODevice::ReadOnly)){
        return;
    }
    QTextStream in(&file);
    QStringList names = in.readLine().split("\t");
    QStringList values = in.readLine().split("\t");
    auto it = names.begin();
    auto iv = values.begin();
    for(;it != names.end()&& iv != values.end(); it++,iv++) {
        ui->listWidget->insertItem(ui->listWidget->count(),*it+"\t"+*iv);
    }
}
void lineGraph::on_chooseGraphs_clicked() {
    ChooseGraphs* choose = new ChooseGraphs(ui->plot, this);
    choose->setWindowTitle(this->yLabel);
    choose->exec();
}
void lineGraph::on_toggleLegend_clicked() {
    ui->plot->legend->setVisible(!ui->plot->legend->visible());
    ui->plot->replot();
}
