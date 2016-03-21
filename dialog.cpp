#include "dialog.h"
#include "ui_dialog.h"
//#include "ioevent.h"
#include "heart_cell_sim.h"

#include <QIODevice>
#include <QList>
#include <QGridLayout>
#include <QMap>

QString root = "./build-MyConcurrentModel-Desktop_Qt_5_5_1_MinGW_32bit-Debug";
bool gr;
Dialog::Dialog(Protocol* inital_proto, QDir read_location, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
    this->proto = inital_proto;
    this->read_location = read_location;
    QMessageBox::information(0,"Folder", "Folder named: " + read_location.absolutePath());
    passing_to_graph(this->proto,read_location.absolutePath() + "/dt0_dvars.dat");
}
Dialog::~Dialog()
{
    delete ui;
}
void Dialog::passing_to_graph(Protocol* a, QString f){

     QVector<double> x,y1, y2, y3,y4, y5;
     QVector<double> y6, y7,y8, y9, y10, y11;
     int low_x_axis = 0;
     int max_x_axis = 0;
     int num_vars_graph = 0;

     QFile file(f);
     if(!file.open(QIODevice::ReadOnly)){
         QMessageBox::information(0,"error", "No File Selected!");
         //exit(EXIT_FAILURE);
     }
     QTextStream in(&file);
     //watch problems with first line of labels
     QString fline = in.readLine();
     QStringList split_line = fline.split("\t");
     num_vars_graph = split_line.length();
     while(!in.atEnd()){
         QString line = in.readLine();
         QStringList split_line = line.split("\t");
         y1.push_back(split_line[0].toDouble());
         y2.push_back(split_line[1].toDouble());
         if(num_vars_graph > 3){
             y3.push_back(split_line[2].toDouble());
             if(num_vars_graph > 4){
                 y4.push_back(split_line[3].toDouble());
                 if(num_vars_graph > 5){
                     y5.push_back(split_line[4].toDouble());
                     if(num_vars_graph > 6){
                         y6.push_back(split_line[5].toDouble());
                         if(num_vars_graph > 7){
                             y7.push_back(split_line[6].toDouble());
                             if(num_vars_graph > 8){
                                 y8.push_back(split_line[7].toDouble());
                                 if(num_vars_graph > 9){
                                     y9.push_back(split_line[8].toDouble());
                                     if(num_vars_graph > 10){
                                         y10.push_back(split_line[9].toDouble());
                                         if(num_vars_graph > 11){
                                             y11.push_back(split_line[10].toDouble());
                                         }
                                     }
                                 }
                             }
                         }
                     }
                 }
             }
         }
     }
     file.close();

     //Need to sort out which variable is time now::
     int pos = split_line.indexOf("t");
     if(pos == -1) {
         QMessageBox::information(0,"ERROR NO TIME", "Time must be selected as Output Variable!");
         exit(EXIT_FAILURE);
     }
     else if(pos == 0){x = y1;}
     else if(pos == 1){ x = y2;}
     else if(pos == 2){ x = y3;}
     else if(pos == 3){ x = y4;}
     else if(pos == 4){ x = y5;}
     else if(pos == 5){ x = y6;}
     else if(pos == 6){ x = y7;}
     else if(pos == 7){ x = y8;}
     else if(pos == 8){ x = y9;}
     else if(pos == 9){ x = y10;}
     else if(pos == 10){ x = y11;}

     low_x_axis = std::stoi(a->pars["writetime"].get());
     max_x_axis = std::stoi(a->pars["tMax"].get());

     ui->plot1->addGraph();
     ui->plot1->graph(0)->setPen(QPen(Qt::blue));
     ui->plot1->graph(0)->setData(x, y1);
     ui->plot1->xAxis->grid()->setVisible(false);
     ui->plot1->yAxis->grid()->setVisible(false);
     ui->plot1->xAxis->setLabelFont(QFont(font().family(), 16));
     ui->plot1->yAxis->setLabelFont(QFont(font().family(), 16));
     ui->plot1->xAxis->setLabel("Time (ms)");
     ui->plot1->yAxis->setLabel(split_line[0]);
     ui->plot1->xAxis->setRange(low_x_axis, max_x_axis);
     ui->plot1->yAxis->rescale();
     ui->plot1->graph(0)->setName("Simulation");
     ui->plot1->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);
     if(ui->plot1->plotLayout()->rowCount() < 2){
        ui->plot1->plotLayout()->insertRow(0);
     }
     QCPPlotTitle *title = new QCPPlotTitle(ui->plot1, split_line[0] +" vs Time");
     ui->plot1->plotLayout()->addElement(0,0, title);


     //For each variable in D-Var there needs to be a graph added
     if(num_vars_graph > 2){

         ui->plot2->addGraph(0);
         ui->plot2->graph(0)->setPen(QPen(Qt::blue));
         ui->plot2->graph(0)->setData(x, y2);
         ui->plot2->xAxis->grid()->setVisible(false);
         ui->plot2->yAxis->grid()->setVisible(false);
         ui->plot2->xAxis->setLabelFont(QFont(font().family(), 16));
         ui->plot2->yAxis->setLabelFont(QFont(font().family(), 16));
         ui->plot2->xAxis->setLabel("Time (ms)");
         ui->plot2->yAxis->setLabel(split_line[1]);
         ui->plot2->xAxis->setRange(low_x_axis, max_x_axis);
         ui->plot2->graph(0)->setName("Simulation");
         ui->plot2->yAxis->rescale();
         ui->plot2->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);
         if(ui->plot2->plotLayout()->rowCount() < 2){
            ui->plot2->plotLayout()->insertRow(0);
         }
         ui->plot2->plotLayout()->addElement(0,0, new QCPPlotTitle(ui->plot2, split_line[1] +" vs Time"));
         ui->plot2->replot();

         if(num_vars_graph > 3){

             ui->plot3->addGraph(0);
             ui->plot3->graph(0)->setPen(QPen(Qt::blue));
             ui->plot3->graph(0)->setData(x, y3);
             ui->plot3->xAxis->grid()->setVisible(false);
             ui->plot3->yAxis->grid()->setVisible(false);
             ui->plot3->xAxis->setLabelFont(QFont(font().family(), 16));
             ui->plot3->yAxis->setLabelFont(QFont(font().family(), 16));
             ui->plot3->xAxis->setLabel("Time (ms)");
             ui->plot3->yAxis->setLabel(split_line[2]);
             ui->plot3->xAxis->setRange(low_x_axis, max_x_axis);
             ui->plot3->yAxis->rescale();
             ui->plot3->graph(0)->setName("Simulation");
             ui->plot3->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);
             if(ui->plot3->plotLayout()->rowCount() < 2){
                ui->plot3->plotLayout()->insertRow(0);
             }
             ui->plot3->plotLayout()->addElement(0,0, new QCPPlotTitle(ui->plot3, split_line[2] +" vs Time"));

             if(num_vars_graph > 4){
                 ui->plot4->addGraph(0);
                 ui->plot4->graph(0)->setPen(QPen(Qt::blue));
                 ui->plot4->graph(0)->setData(x, y4);
                 ui->plot4->xAxis->grid()->setVisible(false);
                 ui->plot4->yAxis->grid()->setVisible(false);
                 ui->plot4->xAxis->setLabelFont(QFont(font().family(), 16));
                 ui->plot4->yAxis->setLabelFont(QFont(font().family(), 16));
                 ui->plot4->xAxis->setLabel("Time (ms)");
                 ui->plot4->yAxis->setLabel(split_line[3]);
                 ui->plot4->xAxis->setRange(low_x_axis, max_x_axis);
                 ui->plot4->yAxis->rescale();
                 ui->plot4->graph(0)->setName("Simulation");
                 ui->plot4->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);
                 if(ui->plot4->plotLayout()->rowCount() < 2){
                    ui->plot4->plotLayout()->insertRow(0);
                 }
                 ui->plot4->plotLayout()->addElement(0,0, new QCPPlotTitle(ui->plot4, split_line[3] +" vs Time"));

                 if(num_vars_graph > 5){
                     ui->plot5->addGraph(0);
                     ui->plot5->graph(0)->setPen(QPen(Qt::blue));
                     ui->plot5->graph(0)->setData(x,y5);
                     ui->plot5->xAxis->grid()->setVisible(false);
                     ui->plot5->yAxis->grid()->setVisible(false);
                     ui->plot5->xAxis->setLabelFont(QFont(font().family(), 16));
                     ui->plot5->yAxis->setLabelFont(QFont(font().family(), 16));
                     ui->plot5->xAxis->setLabel("Time (ms)");
                     ui->plot5->yAxis->setLabel(split_line[4]);
                     ui->plot5->xAxis->setRange(low_x_axis, max_x_axis);
                     ui->plot5->yAxis->rescale();
                     ui->plot5->graph(0)->setName("Simulation");
                     ui->plot5->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);
                     if(ui->plot5->plotLayout()->rowCount() < 2){
                        ui->plot5->plotLayout()->insertRow(0);
                     }
                     ui->plot5->plotLayout()->addElement(0,0, new QCPPlotTitle(ui->plot5, split_line[4] +" vs Time"));

                     if(num_vars_graph > 6){
                         ui->plot6->addGraph(0);
                         ui->plot6->graph(0)->setPen(QPen(Qt::blue));
                         ui->plot6->graph(0)->setData(x,y6);
                         ui->plot6->xAxis->grid()->setVisible(false);
                         ui->plot6->yAxis->grid()->setVisible(false);
                         ui->plot6->xAxis->setLabelFont(QFont(font().family(), 16));
                         ui->plot6->yAxis->setLabelFont(QFont(font().family(), 16));
                         ui->plot6->xAxis->setLabel("Time (ms)");
                         ui->plot6->yAxis->setLabel(split_line[5]);
                         ui->plot6->xAxis->setRange(low_x_axis, max_x_axis);
                         ui->plot6->yAxis->rescale();
                         ui->plot6->graph(0)->setName("Simulation");
                         ui->plot6->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);
                         if(ui->plot6->plotLayout()->rowCount() < 2){
                            ui->plot6->plotLayout()->insertRow(0);
                         }
                         ui->plot6->plotLayout()->addElement(0,0, new QCPPlotTitle(ui->plot6, split_line[5] +" vs Time"));

                         if(num_vars_graph > 7){
                             ui->plot7->addGraph(0);
                             ui->plot7->graph(0)->setPen(QPen(Qt::blue));
                             ui->plot7->graph(0)->setData(x,y7);
                             ui->plot7->xAxis->grid()->setVisible(false);
                             ui->plot7->yAxis->grid()->setVisible(false);
                             ui->plot7->xAxis->setLabelFont(QFont(font().family(), 16));
                             ui->plot7->yAxis->setLabelFont(QFont(font().family(), 16));
                             ui->plot7->xAxis->setLabel("Time (ms)");
                             ui->plot7->yAxis->setLabel(split_line[6]);
                             ui->plot7->xAxis->setRange(low_x_axis, max_x_axis);
                             ui->plot7->yAxis->rescale();
                             ui->plot7->graph(0)->setName("Simulation");
                             ui->plot7->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);
                             if(ui->plot7->plotLayout()->rowCount() < 2){
                                ui->plot7->plotLayout()->insertRow(0);
                             }
                             ui->plot7->plotLayout()->addElement(0,0, new QCPPlotTitle(ui->plot7, split_line[6] +" vs Time"));

                             if(num_vars_graph > 8){
                                 ui->plot8->addGraph(0);
                                 ui->plot8->graph(0)->setPen(QPen(Qt::blue));
                                 ui->plot8->graph(0)->setData(x,y8);
                                 ui->plot8->xAxis->grid()->setVisible(false);
                                 ui->plot8->yAxis->grid()->setVisible(false);
                                 ui->plot8->xAxis->setLabelFont(QFont(font().family(), 16));
                                 ui->plot8->yAxis->setLabelFont(QFont(font().family(), 16));
                                 ui->plot8->xAxis->setLabel("Time (ms)");
                                 ui->plot8->yAxis->setLabel(split_line[7]);
                                 ui->plot8->xAxis->setRange(low_x_axis, max_x_axis);
                                 ui->plot8->yAxis->rescale();
                                 ui->plot8->graph(0)->setName("Simulation");
                                 ui->plot8->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);
                                 if(ui->plot8->plotLayout()->rowCount() < 2){
                                    ui->plot8->plotLayout()->insertRow(0);
                                 }
                                 ui->plot8->plotLayout()->addElement(0,0, new QCPPlotTitle(ui->plot8, split_line[7] +" vs Time"));

                                 if(num_vars_graph > 9){
                                     ui->plot9->addGraph(0);
                                     ui->plot9->graph(0)->setPen(QPen(Qt::blue));
                                     ui->plot9->graph(0)->setData(x,y9);
                                     ui->plot9->xAxis->grid()->setVisible(false);
                                     ui->plot9->yAxis->grid()->setVisible(false);
                                     ui->plot9->xAxis->setLabelFont(QFont(font().family(), 16));
                                     ui->plot9->yAxis->setLabelFont(QFont(font().family(), 16));
                                     ui->plot9->xAxis->setLabel("Time (ms)");
                                     ui->plot9->yAxis->setLabel(split_line[8]);
                                     ui->plot9->xAxis->setRange(low_x_axis, max_x_axis);
                                     ui->plot9->yAxis->rescale();
                                     ui->plot9->graph(0)->setName("Simulation");
                                     ui->plot9->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);
                                     if(ui->plot9->plotLayout()->rowCount() < 2){
                                        ui->plot9->plotLayout()->insertRow(0);
                                     }
                                     ui->plot9->plotLayout()->addElement(0,0, new QCPPlotTitle(ui->plot9, split_line[8] +" vs Time"));

                                     if(num_vars_graph > 10){
                                         ui->plot10->addGraph(0);
                                         ui->plot10->graph(0)->setPen(QPen(Qt::blue));
                                         ui->plot10->graph(0)->setData(x,y10);
                                         ui->plot10->xAxis->grid()->setVisible(false);
                                         ui->plot10->yAxis->grid()->setVisible(false);
                                         ui->plot10->xAxis->setLabelFont(QFont(font().family(), 16));
                                         ui->plot10->yAxis->setLabelFont(QFont(font().family(), 16));
                                         ui->plot10->xAxis->setLabel("Time (ms)");
                                         ui->plot10->yAxis->setLabel(split_line[9]);
                                         ui->plot10->xAxis->setRange(low_x_axis, max_x_axis);
                                         ui->plot10->yAxis->rescale();
                                         ui->plot10->graph(0)->setName("Simulation");
                                         ui->plot10->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);
                                         if(ui->plot10->plotLayout()->rowCount() < 2){
                                            ui->plot10->plotLayout()->insertRow(0);
                                         }
                                         ui->plot10->plotLayout()->addElement(0,0, new QCPPlotTitle(ui->plot10, split_line[9] +" vs Time"));

                                         if(num_vars_graph > 11){
                                             ui->plot11->addGraph(0);
                                             ui->plot11->graph(0)->setPen(QPen(Qt::blue));
                                             ui->plot11->graph(0)->setData(x,y11);
                                             ui->plot11->xAxis->grid()->setVisible(false);
                                             ui->plot11->yAxis->grid()->setVisible(false);
                                             ui->plot11->xAxis->setLabelFont(QFont(font().family(), 16));
                                             ui->plot11->yAxis->setLabelFont(QFont(font().family(), 16));
                                             ui->plot11->xAxis->setLabel("Time (ms)");
                                             ui->plot11->yAxis->setLabel(split_line[10]);
                                             ui->plot11->xAxis->setRange(low_x_axis, max_x_axis);
                                             ui->plot11->yAxis->rescale();
                                             ui->plot11->graph(0)->setName("Simulation");
                                             ui->plot11->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);
                                             if(ui->plot11->plotLayout()->rowCount() < 2){
                                                ui->plot11->plotLayout()->insertRow(0);
                                             }
                                             ui->plot11->plotLayout()->addElement(0,0, new QCPPlotTitle(ui->plot11, split_line[10] +" vs Time"));
                                         }
                                     }
                                 }
                             }
                         }
                     }
                 }
             }
         }
     }
 }
void Dialog::on_pushButton_clicked()
{
    QVector<double> x(6000), y(6000);
    QString toGet = ui->plot1->yAxis->label();
    QString time = "t";

    gr = control_on_graph(x, y, toGet, time);
    if (gr){
        ui->plot1->addGraph();
        ui->plot1->graph(1)->setData(x, y);
        ui->plot1->graph(1)->setPen(QPen(Qt::red));
        ui->plot1->graph(1)->setName("Control");
        ui->plot1->yAxis->rescale();
        ui->plot1->legend->setVisible(true);
        ui->plot1->replot();
    }
}
void Dialog::on_pushButton_2_clicked()
{
    QVector<double> x(6000), y(6000);
    QString toGet = ui->plot2->yAxis->label();
    QString time = "t";

    gr = control_on_graph(x, y, toGet, time);
    if(gr){
        ui->plot2->addGraph();
        ui->plot2->graph(1)->setData(x, y);
        ui->plot2->graph(1)->setPen(QPen(Qt::red));
        ui->plot2->graph(1)->setName("Control");
        ui->plot2->yAxis->rescale();
        ui->plot2->legend->setVisible(true);
          ui->plot2->replot();
    }
}
void Dialog::on_pushButton_3_clicked()
{
    QVector<double> x(6000), y(6000);
    QString toGet = ui->plot3->yAxis->label();
    QString time = "t";

    gr = control_on_graph(x, y, toGet, time);
    if(gr){
        ui->plot3->addGraph();
        ui->plot3->graph(1)->setData(x, y);
        ui->plot3->graph(1)->setPen(QPen(Qt::red));
        ui->plot3->graph(1)->setName("Control");
        ui->plot3->yAxis->rescale();
        ui->plot3->legend->setVisible(true);
        ui->plot3->replot();
    }
}
void Dialog::on_pushButton_4_clicked()
{
    QVector<double> x(6000), y(6000);
    QString toGet = ui->plot4->yAxis->label();
    QString time = "t";

    gr = control_on_graph(x, y, toGet, time);
    if(gr){
        ui->plot4->addGraph();
        ui->plot4->graph(1)->setData(x, y);
        ui->plot4->graph(1)->setPen(QPen(Qt::red));
        ui->plot4->graph(1)->setName("Control");
        ui->plot4->yAxis->rescale();
        ui->plot4->legend->setVisible(true);
        ui->plot4->replot();
    }
}
void Dialog::on_pushButton_5_clicked()
{
    QVector<double> x(6000), y(6000);
    QString toGet = ui->plot5->yAxis->label();
    QString time = "t";

    gr = control_on_graph(x, y, toGet, time);
    if(gr){
        ui->plot5->addGraph();
        ui->plot5->graph(1)->setData(x, y);
        ui->plot5->graph(1)->setPen(QPen(Qt::red));
        ui->plot5->graph(1)->setName("Control");
        ui->plot5->yAxis->rescale();
        ui->plot5->legend->setVisible(true);
        ui->plot5->replot();
    }
}
void Dialog::on_pushButton_6_clicked()
{
    QVector<double> x(6000), y(6000);
    QString toGet = ui->plot6->yAxis->label();
    QString time = "t";

    gr= control_on_graph(x, y, toGet, time);
    if(gr){ui->plot6->addGraph();
        ui->plot6->graph(1)->setData(x, y);
        ui->plot6->graph(1)->setPen(QPen(Qt::red));
        ui->plot6->graph(1)->setName("Control");
        ui->plot6->yAxis->rescale();
        ui->plot6->legend->setVisible(true);
        ui->plot6->replot();
    }
}
void Dialog::on_pushButton_7_clicked()
{
    QVector<double> x(6000), y(6000);
    QString toGet = ui->plot7->yAxis->label();
    QString time = "t";

    gr = control_on_graph(x, y, toGet, time);
    if(gr){
        ui->plot7->addGraph();
        ui->plot7->graph(1)->setData(x, y);
        ui->plot7->graph(1)->setPen(QPen(Qt::red));
        ui->plot7->graph(1)->setName("Control");
        ui->plot7->yAxis->rescale();
        ui->plot7->legend->setVisible(true);
        ui->plot7->replot();
    }
}
void Dialog::on_pushButton_8_clicked()
{
    QVector<double> x(6000), y(6000);
    QString toGet = ui->plot8->yAxis->label();
    QString time = "t";

    gr = control_on_graph(x, y, toGet, time);
    if(gr){ui->plot8->addGraph();
        ui->plot8->graph(1)->setData(x, y);
        ui->plot8->graph(1)->setPen(QPen(Qt::red));
        ui->plot8->graph(1)->setName("Control");
        ui->plot8->yAxis->rescale();
        ui->plot8->legend->setVisible(true);
        ui->plot8->replot();
    }
}
void Dialog::on_pushButton_9_clicked()
{
    QVector<double> x(6000), y(6000);
    QString toGet = ui->plot9->yAxis->label();
    QString time = "t";

    gr = control_on_graph(x, y, toGet, time);
    if(gr){ui->plot9->addGraph();
        ui->plot9->graph(1)->setData(x, y);
        ui->plot9->graph(1)->setPen(QPen(Qt::red));
        ui->plot9->graph(1)->setName("Control");
        ui->plot9->yAxis->rescale();
        ui->plot9->legend->setVisible(true);
        ui->plot9->replot();
    }
}
void Dialog::on_pushButton_10_clicked()
{
    QVector<double> x(6000), y(6000);
    QString toGet = ui->plot10->yAxis->label();
    QString time = "t";

    gr =control_on_graph(x, y, toGet, time);;
    if(gr){ui->plot10->addGraph();
        ui->plot10->graph(1)->setData(x, y);
        ui->plot10->graph(1)->setPen(QPen(Qt::red));
        ui->plot10->graph(1)->setName("Control");
        ui->plot10->yAxis->rescale();
        ui->plot10->legend->setVisible(true);
        ui->plot10->replot();
    }
}
void Dialog::on_pushButton_11_clicked()
{
    QVector<double> x(6000), y(6000);
    QString toGet = ui->plot11->yAxis->label();
    QString time = "t";

    gr= control_on_graph(x, y, toGet, time);
    if(gr){ui->plot11->addGraph();
        ui->plot11->graph(1)->setData(x, y);
        ui->plot11->graph(1)->setPen(QPen(Qt::red));
        ui->plot11->graph(1)->setName("Control");
        ui->plot11->yAxis->rescale();
        ui->plot11->legend->setVisible(true);
        ui->plot11->replot();
    }
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
        y[point] = split_line[varpos].toDouble();
        x[point] = split_line[timepos].toDouble();
        point++;
    }
    file1.close();
    return true;
    }
    return false;
}
void Dialog::on_save1_clicked()
{
    ui->plot1->saveJpg(read_location.absolutePath() + ui->plot1->yAxis->label() + "/vsTime.jpg", 0,0,1.0, -1);
}
void Dialog::on_save2_clicked()
{
    ui->plot2->saveJpg(read_location.absolutePath() + ui->plot2->yAxis->label() + "/vsTime.jpg", 0,0,1.0, -1);
}
void Dialog::on_save3_clicked()
{
    ui->plot3->saveJpg(read_location.absolutePath() + ui->plot3->yAxis->label() + "/vsTime.jpg", 0,0,1.0, -1);
}
void Dialog::on_save4_clicked()
{
    ui->plot4->saveJpg(read_location.absolutePath() + ui->plot4->yAxis->label() + "/vsTime.jpg", 0,0,1.0, -1);
}
void Dialog::on_save5_clicked()
{
    ui->plot5->saveJpg(read_location.absolutePath() + ui->plot5->yAxis->label() + "/vsTime.jpg", 0,0,1.0, -1);
}
void Dialog::on_save6_clicked()
{
    ui->plot6->saveJpg(read_location.absolutePath() + ui->plot6->yAxis->label() + "/vsTime.jpg", 0,0,1.0, -1);
}
void Dialog::on_save7_clicked()
{
    ui->plot7->saveJpg(read_location.absolutePath() + ui->plot7->yAxis->label() + "/vsTime.jpg", 0,0,1.0, -1);
}
void Dialog::on_save8_clicked()
{
    ui->plot8->saveJpg(read_location.absolutePath() + ui->plot8->yAxis->label() + "/vsTime.jpg", 0,0,1.0, -1);
}
void Dialog::on_save9_clicked()
{
    ui->plot9->saveJpg(read_location.absolutePath() + ui->plot9->yAxis->label() + "/vsTime.jpg", 0,0,1.0, -1);
}
void Dialog::on_save10_clicked()
{
    ui->plot10->saveJpg(read_location.absolutePath() + ui->plot10->yAxis->label() + "/vsTime.jpg", 0,0,1.0, -1);
}
void Dialog::on_save11_clicked()
{
    ui->plot11->saveJpg(read_location.absolutePath() + ui->plot11->yAxis->label() + "/vsTime.jpg", 0,0,1.0, -1);
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

        passing_to_graph(this->proto, filename);
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
