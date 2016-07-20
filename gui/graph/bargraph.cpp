#include "bargraph.h"
#include "ui_bargraph.h"
#include <QInputDialog>

barGraph::barGraph(QString name, double value, QString var, QDir saveDir, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::barGraph)
{
    ui->setupUi(this);
    this->saveDir = saveDir;
    bar newBar;
    newBar.data.append(value);
    labels.append(name);
    aspect = name;
    this->var = var;
    this->Initialize();
    this->newBar(newBar);
    QCPRange range(0,newBar.data.first()*1.2);
    range.normalize();
    ui->plot->yAxis->setRange(range);
}
void barGraph::Initialize() {
    ui->plot->plotLayout()->addElement(1,0, new QCPPlotTitle(ui->plot, var));
//    valueBar->setName(name);
    ui->plot->xAxis->setAutoTicks(false);
    ui->plot->xAxis->setAutoTickLabels(false);
//    QPen gridPen;
//    gridPen.setStyle(Qt::SolidLine);
//    gridPen.setColor(QColor(0, 0, 0, 25));
//    ui->plot->yAxis->grid()->setPen(gridPen);
//    gridPen.setStyle(Qt::DotLine);
//    ui->plot->yAxis->grid()->setSubGridPen(gridPen); 
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
    bar newBar;
    QTextStream in(&file);
    QStringList names = in.readLine().split("\t", QString::SkipEmptyParts);
    QStringList values = in.readLine().split("\t", QString::SkipEmptyParts);
    auto it = names.begin();
    auto iv = values.begin();
   for(;it != names.end()&& iv != values.end(); it++,iv++) {
        if(aspect == *it) {
            bool ok;
            QString name = QInputDialog::getText(this, tr("Other Simulation Name"), tr("Name:"), QLineEdit::Normal, *it, &ok);
            if(!ok) {
                name = *it;
            }
            labels.append(name);
            newBar.data.append(iv->toDouble());
        }
    }

    this->newBar(newBar);    
}
void barGraph::setRange(bar newBar) {
    QCPRange oldRange = ui->plot->yAxis->range();
    QCPRange newRange(0, newBar.data.first()*1.2);
    newRange.normalize();
    newRange.expand(oldRange);
    ui->plot->yAxis->setRange(newRange);
}
void barGraph::newBar(bar& newBar) {
    if(labels.empty() || newBar.data.empty()) return;
    newBar.valueBar = new QCPBars(ui->plot->xAxis, ui->plot->yAxis);
    QColor barColor = genColor(ticks.size());
    newBar.valueBar->setPen(QPen(barColor));
    barColor.setAlpha(150);
    newBar.valueBar->setBrush(barColor);
    ui->plot->addPlottable(newBar.valueBar);
    ticks << ticks.size()+1;
    ui->plot->xAxis->setTickVector(ticks);
    ui->plot->xAxis->setTickVectorLabels(labels);
    ui->plot->xAxis->setTickLabelRotation(60);
    ui->plot->xAxis->setSubTickCount(0);
    ui->plot->xAxis->setTickLength(0, 4);
//    ui->plot->xAxis->grid()->setVisible(true);
    ui->plot->xAxis->setRange(0, ticks.size()+1);
    this->setRange(newBar);
    ui->plot->yAxis->setPadding(5); // a bit more space to the left border
//    ui->plot->yAxis->setLabel("Power Consumption in\nKilowatts per Capita (2007)");
//    ui->plot->yAxis->grid()->setSubGridVisible(true);
    
    newBar.valueBar->addData(ticks.size(), newBar.data.at(0));
    ui->plot->replot();
}
void barGraph::on_save_clicked() {
    ui->plot->saveJpg(saveDir.absolutePath() + "/" + var + ".jpg", 0,0,1.0, -1);
}
QColor barGraph::genColor(int numBars) {
    return QColor::fromHsv((numBars*4*17)%360,200,200);
}
