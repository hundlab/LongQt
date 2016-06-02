#include "bargraph.h"
#include "ui_bargraph.h"

barGraph::barGraph(QString name, double value, QString var, QDir saveDir, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::barGraph)
{
    ui->setupUi(this);
    this->saveDir = saveDir;
    this->data << value;
    this->Initialize(name, var);
}
void barGraph::Initialize(QString name, QString var) {
    ui->plot->plotLayout()->addElement(1,0, new QCPPlotTitle(ui->plot, var));
    QCPBars* valueBar = new QCPBars(ui->plot->xAxis, ui->plot->yAxis);
    ui->plot->addPlottable(valueBar);
//    valueBar->setName(name);
    QVector<double> ticks;
    QVector<QString> labels;
    ticks << 1;
    labels << name;
    ui->plot->xAxis->setAutoTicks(false);
    ui->plot->xAxis->setAutoTickLabels(false);
    ui->plot->xAxis->setTickVector(ticks);
    ui->plot->xAxis->setTickVectorLabels(labels);
    ui->plot->xAxis->setTickLabelRotation(60);
    ui->plot->xAxis->setSubTickCount(0);
    ui->plot->xAxis->setTickLength(0, 4);
    ui->plot->xAxis->grid()->setVisible(true);
    ui->plot->xAxis->setRange(0, 2);
    ui->plot->yAxis->setRange(0, data.first()*1.2);
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
}
barGraph::~barGraph()
{
    delete ui;
}
