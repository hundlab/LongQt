#include "dialog.h"
#include "linegraph.h"
#include "bargraph.h"
#include "ui_dialog.h"
#include "heart_cell_sim.h"
#include "measure.h"

#include <QIODevice>
#include <QList>
#include <QGridLayout>
#include <QMap>
#include <exception>

Dialog::Dialog(Protocol* inital_proto, QDir read_location, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
    this->proto = inital_proto;
    this->read_location = read_location;
    this->Initialize();
}
void Dialog::Initialize() {
    buildLineGraphs(read_location.absolutePath() + "/dt0_dvars.dat");
    buildBarGraphs(0);
}
Dialog::~Dialog()
{
    delete ui;
}
void Dialog::buildLineGraphs(QString f){
     QVector<QVector<double>> y;
     QFile file(f);
     int xIndex;
     QStringList split_line;

     if(!file.open(QIODevice::ReadOnly)){
        QMessageBox::information(0,"error", "No File Selected!");
        throw badFile();
     }
     QTextStream in(&file);
     //watch problems with first line of labels
     QString fline = in.readLine();
     split_line = fline.split("\t",QString::SkipEmptyParts);
     y = QVector<QVector<double>>(split_line.length());
     while(!in.atEnd()){
         QStringList values = in.readLine().split("\t", QString::SkipEmptyParts);
         for(int i = 0; i < values.length() && i < split_line.length(); i++) {
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

    for(int i = 0; i < split_line.length(); i++) {
        if(i == xIndex) {
            continue;
        }
        ui->tabWidget->addTab(new lineGraph(this->proto,qMakePair(split_line[xIndex],&y[xIndex]),qMakePair(split_line[i],&y[i]),this->read_location),split_line[i]);        
     }
}
void Dialog::buildBarGraphs(int trial) {
    char filename[1500];
    set<string> vars = proto->cell->getVariables();
    for(auto var : vars) {
        sprintf(filename, proto->finalpropertyoutfile.c_str(), trial, var.c_str());
        QFile file(read_location.absolutePath() +"/"+ QString(filename));
        if(!file.open(QIODevice::ReadOnly)){
            continue;
        }
        QTextStream in(&file);
        QStringList names = in.readLine().split("\t", QString::SkipEmptyParts);
        QStringList values = in.readLine().split("\t", QString::SkipEmptyParts);
        auto name = names.begin();
        auto value = values.begin();
        for(;name != names.end()&& value != values.end(); name++,value++) {
            ui->tabWidget->addTab(new barGraph(*name, value->toDouble(), var.c_str(), read_location), QString(var.c_str()) +":"+QString(*name));
        }
    }
}
void Dialog::on_loadNew_clicked()
{
    QString filename = QFileDialog::getOpenFileName(
                this,
                tr("Open File"),
                read_location.absolutePath(),
                "All Files (*.*);;Text File(*.txt)"
                );

    if(!filename.isEmpty()){

        while(ui->tabWidget->count() > 0) {
            ui->tabWidget->removeTab(0);
        }
        buildLineGraphs(filename);
   }
}

