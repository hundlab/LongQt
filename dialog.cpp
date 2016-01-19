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

    ui->plot->addGraph();
    ui->plot->graph(0)->setPen(QPen(Qt::blue));
    ui->plot->graph(0)->setBrush(QBrush(QColor(0, 0, 255, 20))); // first graph will be filled with translucent blue
    ui->plot->addGraph();
    ui->plot->graph(1)->setPen(QPen(Qt::red));


    int size = 6000;                            //may vary- Dani Watch
    QVector<double> x(size), y(size), y0(size);
    int low_x_axis = 0;
    int max_x_axis = 0;


    /**
     * Getting data from simulation for graphing for 1st Plot
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
        y0[point] = split_line[2].toDouble() + 10;  //because I don't have different inputs, changed it by 10
        point++;
    }
    file.close();

    /**
     * Setting  boundaries of x axis
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

    ui->plot->graph(0)->setData(x, y);
    ui->plot->graph(1)->setData(x, y0);
    ui->plot->xAxis->grid()->setVisible(false);
    ui->plot->yAxis->grid()->setVisible(false);
    ui->plot->xAxis->setLabelFont(QFont(font().family(), 16));
    ui->plot->yAxis->setLabelFont(QFont(font().family(), 16));
    ui->plot->xAxis->setLabel("Time (ms)");
    ui->plot->yAxis->setLabel("Voltage (mv)");
    ui->plot->xAxis->setRange(low_x_axis, max_x_axis);
    ui->plot->yAxis->setRange(-60, 40);             //brittle-need to pull in details from file
    ui->plot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);
}
Dialog::~Dialog()
{
    delete ui;
}
