#include "dialog.h"
#include "ui_dialog.h"
#include "ioevent.h"

#include <QIODevice>
#include <QList>

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);

    int size = 6000;                            //may vary- Dani Watch
    QVector<double> x(size), y(size);
    int low_x_axis = 0;
    int max_x_axis = 0;

    /**
     * Gettiing data from simulation for graphing
     */
    QFile file("./data/dtestdocy.txt");
    if(!file.open(QIODevice::ReadOnly)){
        QMessageBox::information(0,"error", file.errorString());
    }
    QTextStream in(&file);
    int point = 0;
    //watch problems with first line of labels
    while(!in.atEnd()){
        QString line = in.readLine();
        QStringList split_line = line.split("\t");
        x[point] = split_line[1].toDouble();
        y[point] = split_line[2].toDouble();
        point++;
    }
    file.close();

    /**
     *  Getting data from Simvar file for xaxis information
     */
    QFile simfile("C:/Users/bec100/Desktop/Example_vars/simvars.txt");
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


    ui->plot->addGraph();
    ui->plot->graph(0)->setData(x,y);
    ui->plot->xAxis->grid()->setVisible(false);
    ui->plot->yAxis->grid()->setVisible(false);
    ui->plot->xAxis->setLabelFont(QFont(font().family(), 16));
    ui->plot->yAxis->setLabelFont(QFont(font().family(), 16));
    ui->plot->xAxis->setLabel("Time (ms)");
    ui->plot->yAxis->setLabel("Voltage (mv)");
    ui->plot->xAxis->setRange(low_x_axis, max_x_axis);        //brittle-need to pull in from file
    ui->plot->yAxis->setRange(-60, 40);             //brittle-need to pull in details from file
}

Dialog::~Dialog()
{
    delete ui;
}
