#include "dialog.h"
#include "ui_dialog.h"
//#include "ioevent.h"
#include "heart_cell_sim.h"

#include <QIODevice>
#include <QList>
#include <QGridLayout>
#include <QMap>
#include <exception>

QString root = "./build-MyConcurrentModel-Desktop_Qt_5_5_1_MinGW_32bit-Debug";
bool gr;
Dialog::Dialog(Protocol* inital_proto, QDir read_location, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
    this->proto = inital_proto;
    this->read_location = read_location;
    this->plots.push_back(ui->plot1);
    this->plots.push_back(ui->plot2);
    this->plots.push_back(ui->plot3);
    this->plots.push_back(ui->plot4);
    this->plots.push_back(ui->plot5);
    this->plots.push_back(ui->plot6);
    this->plots.push_back(ui->plot7);
    this->plots.push_back(ui->plot8);
    this->plots.push_back(ui->plot9);
    this->plots.push_back(ui->plot10);
    this->plots.push_back(ui->plot11);
    QMessageBox::information(0,"Folder", "Simulation is finished!\n The folder is named: " + read_location.absolutePath());
    passing_to_graph(read_location.absolutePath() + "/dt0_dvars.dat");
}
Dialog::~Dialog()
{
    delete ui;
}
void Dialog::passing_to_graph(QString f){

     y = QVector<QVector<double>>(10);

     QFile file(f);
     if(!file.open(QIODevice::ReadOnly)){
        QMessageBox::information(0,"error", "No File Selected!");
        throw badFile();
     }
     QTextStream in(&file);
     //watch problems with first line of labels
     QString fline = in.readLine();
     split_line = fline.split("\t",QString::SkipEmptyParts);
     while(!in.atEnd()){
         QStringList values = in.readLine().split("\t", QString::SkipEmptyParts);
         for(int i = 0; i < values.length() && i < split_line.length() && i < 11; i++) {
             y[i].push_back(values[i].toDouble());
         }
     }
     file.close();

     //Need to sort out which variable is time now::
     xIndex = split_line.indexOf("t");
     if(xIndex == -1) {
         QMessageBox::information(0,"ERROR NO TIME", "Time must be selected as Output Variable!");
         throw badFile();
     }

    for(int i = 0; i < split_line.length() && i < 11; i++) {
/*        if(i == xIndex) {
            continue;
        }*/
        this->InitializeTab(i);
     }
}
void Dialog::loadControl(int num) {
    QVector<double> x, y;
    QString toGet = plots[num]->yAxis->label();
    QString time = "t";
    auto plot = plots[num];

    gr = control_on_graph(x, y, toGet, time);
    if (gr){
        plot->addGraph();
        plot->graph(1)->setData(x, y);
        plot->graph(1)->setPen(QPen(Qt::red));
        plot->graph(1)->setName("Control");
        plot->yAxis->rescale();
        plot->legend->setVisible(true);
        plot->replot();
    }
   
}
void Dialog::on_pushButton_clicked()
{
    loadControl(0);
}
void Dialog::on_pushButton_2_clicked()
{
    loadControl(1);
}
void Dialog::on_pushButton_3_clicked()
{
    loadControl(2);
}
void Dialog::on_pushButton_4_clicked()
{
    loadControl(3);
}
void Dialog::on_pushButton_5_clicked()
{
    loadControl(4);
}
void Dialog::on_pushButton_6_clicked()
{
    loadControl(5);
}
void Dialog::on_pushButton_7_clicked()
{
    loadControl(6);
}
void Dialog::on_pushButton_8_clicked()
{
    loadControl(7);
}
void Dialog::on_pushButton_9_clicked()
{
    loadControl(8);
}
void Dialog::on_pushButton_10_clicked()
{
    loadControl(9);
}
void Dialog::on_pushButton_11_clicked()
{
    loadControl(10);
}
bool Dialog::control_on_graph(QVector<double> &x, QVector<double> &y, QString toGet, QString time){
    QString filename = QFileDialog::getOpenFileName(
                this,
                tr("Open File"),
                root,
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
    int varpos = split_line.indexOf(toGet);
    if(varpos == -1){ QMessageBox::information(0,"Error", toGet +" not found!"); return false;}
    int timepos = split_line.indexOf(time);
    if(timepos == -1){ QMessageBox::information(0,"Error", time +" not found!"); return false;}
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
void Dialog::save(int num) {
    plots[num]->saveJpg(read_location.absolutePath() + "/" + plots[num]->yAxis->label() + "vsTime.jpg", 0,0,1.0, -1);
}
void Dialog::on_save1_clicked()
{
    save(0);
}
void Dialog::on_save2_clicked()
{
    save(1);
}
void Dialog::on_save3_clicked()
{
    save(2);
}
void Dialog::on_save4_clicked()
{
    save(3);
}
void Dialog::on_save5_clicked()
{
    save(4);
}
void Dialog::on_save6_clicked()
{
    save(5);
}
void Dialog::on_save7_clicked()
{
    save(6);
}
void Dialog::on_save8_clicked()
{
    save(7);
}
void Dialog::on_save9_clicked()
{
    save(8);
}
void Dialog::on_save10_clicked()
{
    save(9);
}
void Dialog::on_save11_clicked()
{
    save(10);
}
void Dialog::on_pushButton_12_clicked()
{
    QString filename = QFileDialog::getOpenFileName(
                this,
                tr("Open File"),
                root,
                "All Files (*.*);;Text File(*.txt)"
                );

    if(!filename.isEmpty()){

        ui->plot1->removeGraph(0);
        ui->plot1->removeGraph(1);
        ui->plot2->removeGraph(0);
        ui->plot2->removeGraph(1);
        ui->plot3->removeGraph(0);
        ui->plot3->removeGraph(1);
        ui->plot4->removeGraph(0);
        ui->plot4->removeGraph(1);
        ui->plot5->removeGraph(0);
        ui->plot5->removeGraph(1);
        ui->plot6->removeGraph(0);
        ui->plot6->removeGraph(1);
        ui->plot7->removeGraph(0);
        ui->plot7->removeGraph(1);
        ui->plot8->removeGraph(0);
        ui->plot8->removeGraph(1);
        ui->plot9->removeGraph(0);
        ui->plot9->removeGraph(1);
        ui->plot10->removeGraph(0);
        ui->plot10->removeGraph(1);
        ui->plot11->removeGraph(0);
        ui->plot11->removeGraph(1);

        ui->plot1->legend->clear();
        ui->plot1->legend->setVisible(false);
        ui->plot2->legend->clear();
        ui->plot2->legend->setVisible(false);
        ui->plot3->legend->clear();
        ui->plot3->legend->setVisible(false);
        ui->plot4->legend->clear();
        ui->plot4->legend->setVisible(false);
        ui->plot5->legend->clear();
        ui->plot5->legend->setVisible(false);
        ui->plot6->legend->clear();
        ui->plot6->legend->setVisible(false);
        ui->plot7->legend->clear();
        ui->plot7->legend->setVisible(false);
        ui->plot8->legend->clear();
        ui->plot8->legend->setVisible(false);
        ui->plot9->legend->clear();
        ui->plot9->legend->setVisible(false);
        ui->plot10->legend->clear();
        ui->plot10->legend->setVisible(false);
        ui->plot11->legend->clear();
        ui->plot11->legend->setVisible(false);
        passing_to_graph(filename);

        ui->plot1->replot();
        ui->plot2->replot();
        ui->plot3->replot();
        ui->plot4->replot();
        ui->plot5->replot();
        ui->plot6->replot();
        ui->plot7->replot();
        ui->plot8->replot();
        ui->plot9->replot();
        ui->plot10->replot();
        ui->plot11->replot();
    }
}
void Dialog::populateList(QString varname, int trial, QListWidget* list) {
    char filename[1500];
    sprintf(filename, proto->finalpropertyoutfile.c_str(), trial, varname.toStdString().c_str());
    QFile file(read_location.absolutePath() +"/"+ QString(filename));
    if(!file.open(QIODevice::ReadOnly)){
        return;
    }
    QTextStream in(&file);
    QStringList names = in.readLine().split("\t");
    QStringList values = in.readLine().split("\t");
    auto it = names.begin();
    auto iv = values.begin();
    for(;it != names.end()&& iv != values.end(); it++,iv++) {
        list->insertItem(list->count(),*it+"\t"+*iv);
    }
}
void Dialog::InitializeTab(int num) {
     auto plot = plots[num];
     populateList(split_line[num], 0, ui->listWidget);
     ui->tabWidget->setTabText(num, split_line[num]);
     plot->addGraph();
     plot->graph(0)->setPen(QPen(Qt::blue));
     plot->graph(0)->setData(y[xIndex], y[num]);
     plot->xAxis->grid()->setVisible(false);
     plot->yAxis->grid()->setVisible(false);
     plot->xAxis->setLabelFont(QFont(font().family(), 16));
     plot->yAxis->setLabelFont(QFont(font().family(), 16));
     plot->xAxis->setLabel("Time (ms)");
     plot->yAxis->setLabel(split_line[num]);
     plot->xAxis->setRange(y[xIndex].first(), y[xIndex].last());
     plot->graph(0)->setName("Simulation");
     plot->yAxis->rescale();
     plot->setInteractions(QCP::iRangeZoom | QCP::iRangeDrag);
     plot->axisRect()->setRangeZoom(plot->xAxis->orientation());
     plot->axisRect()->setRangeDrag(plot->xAxis->orientation());

     if(plot->plotLayout()->rowCount() < 2){
        plot->plotLayout()->insertRow(0);
     }
     if(plot->plotLayout()->elementAt(0) != 0) {
        plot->plotLayout()->removeAt(0);
     }
     plot->plotLayout()->addElement(0,0, new QCPPlotTitle(plot, split_line[num] +" vs Time"));
}
