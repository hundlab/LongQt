#include "bargraph.h"
#include "ui_bargraph.h"

barGraph::barGraph(QString name, double value, QString var, QDir saveDir, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::barGraph)
{
    ui->setupUi(this);
    this->saveDir = saveDir;
    this->data.append(value);
    this->labels.append(name);
    this->var = var;
    this->Initialize();
}
void barGraph::Initialize() {
    ui->plot->plotLayout()->addElement(1,0, new QCPPlotTitle(ui->plot, var));
    valueBar = new QCPBars(ui->plot->xAxis, ui->plot->yAxis);
    ui->plot->addPlottable(valueBar);
//    valueBar->setName(name);
    ui->plot->xAxis->setAutoTicks(false);
    ui->plot->xAxis->setAutoTickLabels(false);
    this->newBar();
}
barGraph::~barGraph()
{
    delete ui;
}
void barGraph::on_loadOtherTrial_clicked() {
    QString filename = QFileDialog::getOpenFileName(
                this,
                tr("Open File"),
                saveDir.absolutePath(),
                "All Files (*.*);;Text File(*.txt)"
                );
    QFile file(filename);
    if(!file.open(QIODevice::ReadOnly)){
        return;
    }
    QTextStream in(&file);
    QStringList names = in.readLine().split("\t", QString::SkipEmptyParts);
    QStringList values = in.readLine().split("\t", QString::SkipEmptyParts);
    auto it = names.begin();
    auto iv = values.begin();
    for(;it != names.end()&& iv != values.end(); it++,iv++) {
        if(labels.first() == *it) {
            labels.append(*it);
            data.append(iv->toDouble());
        }
    }

    this->newBar();    
}
void barGraph::newBar() {
    double max = data.first();
    ticks << ticks.size()+1;
    ui->plot->xAxis->setTickVector(ticks);
    ui->plot->xAxis->setTickVectorLabels(labels);
    ui->plot->xAxis->setTickLabelRotation(60);
    ui->plot->xAxis->setSubTickCount(0);
    ui->plot->xAxis->setTickLength(0, 4);
    ui->plot->xAxis->grid()->setVisible(true);
    ui->plot->xAxis->setRange(0, ticks.size()+1);
    for(double d : data) {
        if(max < d) {
            max = d;
        }
    }
    ui->plot->yAxis->setRange(0, max*1.2);
    ui->plot->yAxis->setPadding(5); // a bit more space to the left border
//    ui->plot->yAxis->setLabel("Power Consumption in\nKilowatts per Capita (2007)");
    ui->plot->yAxis->grid()->setSubGridVisible(true);
    QPen gridPen;
    gridPen.setStyle(Qt::SolidLine);
    gridPen.setColor(QColor(0, 0, 0, 25));
    ui->plot->yAxis->grid()->setPen(gridPen);
    gridPen.setStyle(Qt::DotLine);
    ui->plot->yAxis->grid()->setSubGridPen(gridPen); 
    valueBar->setData(ticks, data);
    ui->plot->replot();
}
void barGraph::on_save_clicked() {
    ui->plot->saveJpg(saveDir.absolutePath() + "/" + var + ".jpg", 0,0,1.0, -1);
}
