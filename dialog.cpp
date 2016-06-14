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
    this->buildLineGraphs(this->getFileNames());
    this->buildBarGraphs(0);
}
QFileInfoList Dialog::getFileNames() {
    QFileInfoList toReturn;
    for(QFileInfo file : this->read_location.entryInfoList()) {
        if(QRegExp("cell_\\d+_\\d+_dt\\d+_dvars|dt\\d+_dvars").exactMatch(file.baseName())) {
            toReturn << file;
        }
    }
    return toReturn;
}
QString Dialog::getName(QFileInfo file) {
    QStringList nums;
    nums = file.baseName().split(QRegExp("\\D+"), QString::SkipEmptyParts);
    if(nums.length() > 1) {
        return "Cell " + nums[0] + " " + nums[1];
    } else if( nums.length() > 0) {
        return "Trial " + nums[0];
    } else {
        return "Cell";
    }
}
Dialog::~Dialog()
{
    delete ui;
}
void Dialog::buildLineGraphs(QFileInfoList files){
    QMap<QString, lineGraph*> graphMap;
    for(QFileInfo fileInfo : files) {
        QFile file(fileInfo.absoluteFilePath());
        if(!file.open(QIODevice::ReadOnly)){
            QMessageBox::information(0,"error", "No File Selected!");
            throw badFile();
        }
        QTextStream in(&file);
        QVector<QVector<double>> y;
        int xIndex;
        QStringList split_line;
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
        for(int i = 0; i < split_line.length();i++) {
            if(i == xIndex) {
                continue;
            }
            if(!graphMap.contains(split_line[i])) {
                auto graph = new lineGraph(this->proto,split_line[xIndex],split_line[i],this->read_location);
                graphMap.insert(split_line[i],graph);
                ui->tabWidget->addTab(graph,split_line[i]);
            }
            graphMap[split_line[i]]->addData(y[xIndex],y[i],this->getName(fileInfo));
        }
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
        QFileInfoList filenames;
        filenames << QFileInfo(filename);
        buildLineGraphs(filenames);
   }
}

