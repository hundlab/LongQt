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

    QVector<double> x(size),y1(size), y2(size), y3(size),y4(size), y5(size);
    QVector<double> y6(size), y7(size),y8(size), y9(size), y10(size), y11(size);
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
        y1[point] = split_line[0].toDouble();
                y2[point] = split_line[1].toDouble();
                if(num_vars_graph > 3){
                    y3[point] = split_line[2].toDouble();
                    if(num_vars_graph > 4){
                        y4[point] = split_line[3].toDouble();
                        if(num_vars_graph > 5){
                            y5[point] = split_line[4].toDouble();
                            if(num_vars_graph > 6){
                                y6[point] = split_line[5].toDouble();
                                if(num_vars_graph > 7){
                                    y7[point] = split_line[6].toDouble();
                                    if(num_vars_graph > 8){
                                        y8[point] = split_line[7].toDouble();
                                        if(num_vars_graph > 9){
                                            y9[point] = split_line[8].toDouble();
                                            if(num_vars_graph > 10){
                                                y10[point] = split_line[9].toDouble();
                                                if(num_vars_graph > 11){
                                                    y11[point] = split_line[10].toDouble();
                                                }
                                            }
                                    }
                                }
                                }
                            }
                        }
                    }
                }
        point++;
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
    ui->plot1->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);
    ui->plot1->plotLayout()->insertRow(0);
    ui->plot1->plotLayout()->addElement(0,0, new QCPPlotTitle(ui->plot1, split_line[0] +" vs Time"));

    //For each variable in D-Var there needs to be a grpah added
    if(num_vars_graph > 2){

        ui->plot2->addGraph(0);
        ui->plot2->graph(0)->setPen(QPen(Qt::red));
        ui->plot2->graph(0)->setData(x, y2);
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

        if(num_vars_graph > 3){

            ui->plot3->addGraph(0);
            ui->plot3->graph(0)->setPen(QPen(Qt::green));
            ui->plot3->graph(0)->setData(x, y3);
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


            if(num_vars_graph > 4){

                ui->plot4->addGraph(0);
                ui->plot4->graph(0)->setPen(QPen(Qt::green));
                ui->plot4->graph(0)->setData(x, y4);
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

                if(num_vars_graph > 5){
                    ui->plot5->addGraph(0);
                    ui->plot5->graph(0)->setPen(QPen(Qt::green));
                    ui->plot5->graph(0)->setData(x,y5);
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

                    if(num_vars_graph > 6){
                        ui->plot6->addGraph(0);
                        ui->plot6->graph(0)->setPen(QPen(Qt::green));
                        ui->plot6->graph(0)->setData(x,y6);
                        ui->plot6->xAxis->grid()->setVisible(false);
                        ui->plot6->yAxis->grid()->setVisible(false);
                        ui->plot6->xAxis->setLabelFont(QFont(font().family(), 16));
                        ui->plot6->yAxis->setLabelFont(QFont(font().family(), 16));
                        ui->plot6->xAxis->setLabel("Time (ms)");
                        ui->plot6->yAxis->setLabel(split_line[5]);
                        ui->plot6->xAxis->setRange(low_x_axis, max_x_axis);
                        ui->plot6->yAxis->rescale();
                        ui->plot6->graph(0)->rescaleAxes();
                        ui->plot6->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);
                        ui->plot6->plotLayout()->insertRow(0);
                        ui->plot6->plotLayout()->addElement(0,0, new QCPPlotTitle(ui->plot6, split_line[5] +" vs Time"));

                        if(num_vars_graph > 7){
                            ui->plot7->addGraph(0);
                            ui->plot7->graph(0)->setPen(QPen(Qt::green));
                            ui->plot7->graph(0)->setData(x,y7);
                            ui->plot7->xAxis->grid()->setVisible(false);
                            ui->plot7->yAxis->grid()->setVisible(false);
                            ui->plot7->xAxis->setLabelFont(QFont(font().family(), 16));
                            ui->plot7->yAxis->setLabelFont(QFont(font().family(), 16));
                            ui->plot7->xAxis->setLabel("Time (ms)");
                            ui->plot7->yAxis->setLabel(split_line[6]);
                            ui->plot7->xAxis->setRange(low_x_axis, max_x_axis);
                            ui->plot7->yAxis->rescale();
                            ui->plot7->graph(0)->rescaleAxes();
                            ui->plot7->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);
                            ui->plot7->plotLayout()->insertRow(0);
                            ui->plot7->plotLayout()->addElement(0,0, new QCPPlotTitle(ui->plot7, split_line[6] +" vs Time"));

                            if(num_vars_graph > 8){
                                ui->plot8->addGraph(0);
                                ui->plot8->graph(0)->setPen(QPen(Qt::green));
                                ui->plot8->graph(0)->setData(x,y8);
                                ui->plot8->xAxis->grid()->setVisible(false);
                                ui->plot8->yAxis->grid()->setVisible(false);
                                ui->plot8->xAxis->setLabelFont(QFont(font().family(), 16));
                                ui->plot8->yAxis->setLabelFont(QFont(font().family(), 16));
                                ui->plot8->xAxis->setLabel("Time (ms)");
                                ui->plot8->yAxis->setLabel(split_line[7]);
                                ui->plot8->xAxis->setRange(low_x_axis, max_x_axis);
                                ui->plot8->yAxis->rescale();
                                ui->plot8->graph(0)->rescaleAxes();
                                ui->plot8->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);
                                ui->plot8->plotLayout()->insertRow(0);
                                ui->plot8->plotLayout()->addElement(0,0, new QCPPlotTitle(ui->plot8, split_line[7] +" vs Time"));

                                if(num_vars_graph > 9){
                                    ui->plot9->addGraph(0);
                                    ui->plot9->graph(0)->setPen(QPen(Qt::green));
                                    ui->plot9->graph(0)->setData(x,y9);
                                    ui->plot9->xAxis->grid()->setVisible(false);
                                    ui->plot9->yAxis->grid()->setVisible(false);
                                    ui->plot9->xAxis->setLabelFont(QFont(font().family(), 16));
                                    ui->plot9->yAxis->setLabelFont(QFont(font().family(), 16));
                                    ui->plot9->xAxis->setLabel("Time (ms)");
                                    ui->plot9->yAxis->setLabel(split_line[8]);
                                    ui->plot9->xAxis->setRange(low_x_axis, max_x_axis);
                                    ui->plot9->yAxis->rescale();
                                    ui->plot9->graph(0)->rescaleAxes();
                                    ui->plot9->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);
                                    ui->plot9->plotLayout()->insertRow(0);
                                    ui->plot9->plotLayout()->addElement(0,0, new QCPPlotTitle(ui->plot9, split_line[8] +" vs Time"));

                                    if(num_vars_graph > 10){
                                        ui->plot10->addGraph(0);
                                        ui->plot10->graph(0)->setPen(QPen(Qt::green));
                                        ui->plot10->graph(0)->setData(x,y10);
                                        ui->plot10->xAxis->grid()->setVisible(false);
                                        ui->plot10->yAxis->grid()->setVisible(false);
                                        ui->plot10->xAxis->setLabelFont(QFont(font().family(), 16));
                                        ui->plot10->yAxis->setLabelFont(QFont(font().family(), 16));
                                        ui->plot10->xAxis->setLabel("Time (ms)");
                                        ui->plot10->yAxis->setLabel(split_line[9]);
                                        ui->plot10->xAxis->setRange(low_x_axis, max_x_axis);
                                        ui->plot10->yAxis->rescale();
                                        ui->plot10->graph(0)->rescaleAxes();
                                        ui->plot10->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);
                                        ui->plot10->plotLayout()->insertRow(0);
                                        ui->plot10->plotLayout()->addElement(0,0, new QCPPlotTitle(ui->plot10, split_line[9] +" vs Time"));

                                        if(num_vars_graph > 11){
                                            ui->plot11->addGraph(0);
                                            ui->plot11->graph(0)->setPen(QPen(Qt::green));
                                            ui->plot11->graph(0)->setData(x,y11);
                                            ui->plot11->xAxis->grid()->setVisible(false);
                                            ui->plot11->yAxis->grid()->setVisible(false);
                                            ui->plot11->xAxis->setLabelFont(QFont(font().family(), 16));
                                            ui->plot11->yAxis->setLabelFont(QFont(font().family(), 16));
                                            ui->plot11->xAxis->setLabel("Time (ms)");
                                            ui->plot11->yAxis->setLabel(split_line[10]);
                                            ui->plot11->xAxis->setRange(low_x_axis, max_x_axis);
                                            ui->plot11->yAxis->rescale();
                                            ui->plot11->graph(0)->rescaleAxes();
                                            ui->plot11->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);
                                            ui->plot11->plotLayout()->insertRow(0);
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
Dialog::~Dialog()
{
    delete ui;
}
void Dialog::on_pushButton_clicked()
{
    QString filename = QFileDialog::getOpenFileName(
                this,
                tr("Open File"),
                "C://",
                "All Files (*.*);;Text File(*.txt)"
                );

    QVector<double> x(6000), y(6000);
    QString toGet = ui->plot1->yAxis->label();
    QString time = "t";

    QFile file1(filename);
    if(!file1.open(QIODevice::ReadOnly)){
        QMessageBox::information(0,"error", file1.errorString());
    }
    QTextStream in(&file1);
    int point = 0;
    //watch problems with first line of labels
    QString fline = in.readLine();
    QStringList split_line = fline.split("\t");
    int varpos = split_line.indexOf(toGet);
    int timepos = split_line.indexOf(time);

    while(!in.atEnd()){
        QString line = in.readLine();
        QStringList split_line = line.split("\t");
        y[point] = split_line[varpos].toDouble();
        x[point] = split_line[timepos].toDouble();
        point++;
    }
    file1.close();
    QMessageBox::information(0,"WE HAVE PROBLEMS 2", "HEY DAN");
    ui->plot1->addGraph();
    ui->plot1->graph(1)->setData(x, y);
    ui->plot1->graph(1)->setPen(QPen(Qt::blue));
    ui->plot1->yAxis->rescale();

}
void Dialog::on_pushButton_2_clicked()
{
    QString filename = QFileDialog::getOpenFileName(
                this,
                tr("Open File"),
                "C://",
                "All Files (*.*);;Text File(*.txt)"
                );
}

void Dialog::on_pushButton_3_clicked()
{
    QString filename = QFileDialog::getOpenFileName(
                this,
                tr("Open File"),
                "C://",
                "All Files (*.*);;Text File(*.txt)"
                );
}

void Dialog::on_pushButton_4_clicked()
{
    QString filename = QFileDialog::getOpenFileName(
                this,
                tr("Open File"),
                "C://",
                "All Files (*.*);;Text File(*.txt)"
                );
}

void Dialog::on_pushButton_5_clicked()
{
    QString filename = QFileDialog::getOpenFileName(
                this,
                tr("Open File"),
                "C://",
                "All Files (*.*);;Text File(*.txt)"
                );
}

void Dialog::on_pushButton_6_clicked()
{
    QString filename = QFileDialog::getOpenFileName(
                this,
                tr("Open File"),
                "C://",
                "All Files (*.*);;Text File(*.txt)"
                );
}

void Dialog::on_pushButton_7_clicked()
{
    QString filename = QFileDialog::getOpenFileName(
                this,
                tr("Open File"),
                "C://",
                "All Files (*.*);;Text File(*.txt)"
                );
}

void Dialog::on_pushButton_8_clicked()
{
    QString filename = QFileDialog::getOpenFileName(
                this,
                tr("Open File"),
                "C://",
                "All Files (*.*);;Text File(*.txt)"
                );
}

void Dialog::on_pushButton_9_clicked()
{
    QString filename = QFileDialog::getOpenFileName(
                this,
                tr("Open File"),
                "C://",
                "All Files (*.*);;Text File(*.txt)"
                );
}

void Dialog::on_pushButton_10_clicked()
{
    QString filename = QFileDialog::getOpenFileName(
                this,
                tr("Open File"),
                "C://",
                "All Files (*.*);;Text File(*.txt)"
                );
}

void Dialog::on_pushButton_11_clicked()
{
    QString filename = QFileDialog::getOpenFileName(
                this,
                tr("Open File"),
                "C://",
                "All Files (*.*);;Text File(*.txt)"
                );
}
