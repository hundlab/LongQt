#include "dialog.h"
#include "ui_dialog.h"
#include "ioevent.h"

#include <QIODevice>
#include <QList>
#include <QGridLayout>
#include <QMap>


Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);

    int size = 6000; //may vary- Dani Watch

    QVector<double> x(size), y0(size),y1(size), y2(size), y3(size),y4(size), y5(size) ;
    int low_x_axis = 0;
    int max_x_axis = 0;
    int num_vars_graph = 0;

    /**
     * Getting data from simulation for graphing
     */
    QFile file("C:/Users/bec100/Desktop/Example_vars/dtestdocy.txt");
    //QFile file("./dvars" + QDate::currentDate().toString("MMddyy") + ".txt");
    if(!file.open(QIODevice::ReadOnly)){
        QMessageBox::information(0,"error", file.errorString());
    }
    QTextStream in(&file);
    int point = 0;
    //watch problems with first line of labels
    QString fline = in.readLine();
    QStringList split_line = fline.split("\t");
    num_vars_graph = split_line.length();
    while(!in.atEnd()){
        QString line = in.readLine();
        QStringList split_line = line.split("\t");
        y0[point] = split_line[0].toDouble();    // 1 This is Time
        y1[point] = split_line[1].toDouble();    // 2 This is Voltage
        if(num_vars_graph > 3){
          y2[point] = split_line[2].toDouble();  //0 This is CaI
          if(num_vars_graph > 4){
              y3[point] = split_line[3].toDouble();
              if(num_vars_graph > 5){
                  y4[point] = split_line[4].toDouble();
                  if(num_vars_graph > 6){
                      y5[point] = split_line[5].toDouble();
                  }
              }
          }
        }
        point++;
    }
    file.close();

 //Need to sort out which variable is time now::
   int pos = split_line.indexOf("t");
   if(pos == 0){x = y0;}
   else if(pos == 1){ x = y1;}
   else if(pos == 2){ x = y2;}
   else if(pos == 3){ x = y3;}
   else if(pos == 4){ x = y4;}
   else if(pos == 5){ x = y5;}

    /**
     * Setting  boundaries of y axis
     */
    QFile simfile("./simvars.txt");
    if(!simfile.open(QIODevice::ReadOnly)){
        QMessageBox::information(0,"WE HAVE PROBLEMS 2", simfile.errorString());
    }
    QTextStream simIn(&simfile);
    while(!simIn.atEnd()){
        QString line = simIn.readLine(); //twice to skip the first line (just Labels)
        QStringList split_line = line.split("\t");
        if(split_line[0] == "writetime"){
            low_x_axis = split_line[1].toInt();
        }
        if(split_line[0] == "tMax"){
            max_x_axis = split_line[1].toInt();
        }
    }
    simfile.close();
    if(x != y0){
        ui->plot1->addGraph();
        ui->plot1->graph(0)->setPen(QPen(Qt::blue));
        ui->plot1->graph(0)->setData(x, y0);
        ui->plot1->xAxis->grid()->setVisible(false);
        ui->plot1->yAxis->grid()->setVisible(false);
        ui->plot1->xAxis->setLabelFont(QFont(font().family(), 16));
        ui->plot1->yAxis->setLabelFont(QFont(font().family(), 16));
        ui->plot1->xAxis->setLabel("Time (ms)");
        ui->plot1->yAxis->setLabel(split_line[0]);
        ui->plot1->xAxis->setRange(low_x_axis, max_x_axis);
        ui->plot1->yAxis->rescale();
        ui->plot1->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);
        ui->plot1->plotLayout()->insertRow(0);
        ui->plot1->plotLayout()->addElement(0,0, new QCPPlotTitle(ui->plot1, split_line[0] +" vs Time"));
    }
    //For each variable in D-Var there needs to be a grpah added
    if(num_vars_graph > 2){
        if(x != y1){
            ui->plot2->addGraph(0);
            ui->plot2->graph(0)->setPen(QPen(Qt::red));
            ui->plot2->graph(0)->setData(x, y1);
            ui->plot2->xAxis->grid()->setVisible(false);
            ui->plot2->yAxis->grid()->setVisible(false);
            ui->plot2->xAxis->setLabelFont(QFont(font().family(), 16));
            ui->plot2->yAxis->setLabelFont(QFont(font().family(), 16));
            ui->plot2->xAxis->setLabel("Time (ms)");
            ui->plot2->yAxis->setLabel(split_line[1]);
            ui->plot3->xAxis->setRange(low_x_axis, max_x_axis);
            ui->plot2->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);
            ui->plot2->plotLayout()->insertRow(0);
            ui->plot2->plotLayout()->addElement(0,0, new QCPPlotTitle(ui->plot2, split_line[1] +" vs Time"));
        }

        if(num_vars_graph > 3){
            if( x!= y2){
                ui->plot3->addGraph(0);
                ui->plot3->graph(0)->setPen(QPen(Qt::green));
                ui->plot3->graph(0)->setData(x, y2);
                ui->plot3->xAxis->grid()->setVisible(false);
                ui->plot3->yAxis->grid()->setVisible(false);
                ui->plot3->xAxis->setLabelFont(QFont(font().family(), 16));
                ui->plot3->yAxis->setLabelFont(QFont(font().family(), 16));
                ui->plot3->xAxis->setLabel("Time (ms)");
                ui->plot3->yAxis->setLabel(split_line[2]);
                ui->plot3->xAxis->setRange(low_x_axis, max_x_axis);
                ui->plot3->yAxis->rescale();
                ui->plot3->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);
                ui->plot3->plotLayout()->insertRow(0);
                ui->plot3->plotLayout()->addElement(0,0, new QCPPlotTitle(ui->plot3, split_line[2] +" vs Time"));
            }

            if(num_vars_graph > 4){
                if(x != y3){
                    ui->plot4->addGraph(0);
                    ui->plot4->graph(0)->setPen(QPen(Qt::green));
                    ui->plot4->graph(0)->setData(x, y3);
                    ui->plot4->xAxis->grid()->setVisible(false);
                    ui->plot4->yAxis->grid()->setVisible(false);
                    ui->plot4->xAxis->setLabelFont(QFont(font().family(), 16));
                    ui->plot4->yAxis->setLabelFont(QFont(font().family(), 16));
                    ui->plot4->xAxis->setLabel("Time (ms)");
                    ui->plot4->yAxis->setLabel(split_line[3]);
                    ui->plot4->xAxis->setRange(low_x_axis, max_x_axis);
                    ui->plot4->graph(0)->rescaleAxes();
                    ui->plot4->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);
                    ui->plot4->plotLayout()->insertRow(0);
                    ui->plot4->plotLayout()->addElement(0,0, new QCPPlotTitle(ui->plot4, split_line[3] +" vs Time"));
                }
                if(num_vars_graph > 5){
                    if(x != y4){
                        ui->plot5->addGraph(0);
                        ui->plot5->graph(0)->setPen(QPen(Qt::green));
                        ui->plot5->graph(0)->setData(x,y4);
                        ui->plot5->xAxis->grid()->setVisible(false);
                        ui->plot5->yAxis->grid()->setVisible(false);
                        ui->plot5->xAxis->setLabelFont(QFont(font().family(), 16));
                        ui->plot5->yAxis->setLabelFont(QFont(font().family(), 16));
                        ui->plot5->xAxis->setLabel("Time (ms)");
                        ui->plot5->yAxis->setLabel(split_line[4]);
                        ui->plot5->xAxis->setRange(low_x_axis, max_x_axis);
                        ui->plot5->yAxis->rescale();
                        ui->plot5->graph(0)->rescaleAxes();
                        ui->plot5->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);
                        ui->plot5->plotLayout()->insertRow(0);
                        ui->plot5->plotLayout()->addElement(0,0, new QCPPlotTitle(ui->plot5, split_line[4] +" vs Time"));
                    }
                }
            }
        }
    }
}
Dialog::~Dialog()
{
    delete ui;
}

void Dialog::on_pushButton_clicked()
{

}
