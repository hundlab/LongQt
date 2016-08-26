#include "graph.h"
#include "linegraph.h"
#include "bargraph.h"
#include "ui_graph.h"
#include "heart_cell_sim.h"
#include "measure.h"
#include "loadingprogress.h"

#include <QIODevice>
#include <QList>
#include <QGridLayout>
#include <QMap>
#include <QProgressDialog>
#include <exception>

Dialog::Dialog(QDir read_location, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
    this->read_location = read_location;
    this->Initialize();
}
void Dialog::Initialize() {
    this->buildLineGraphs(this->getFileNames());
    this->buildBarGraphs(0);
}
QFileInfoList Dialog::getFileNames() {
    return this->getFileNames(this->read_location);
}
QFileInfoList Dialog::getFileNames(QDir location) {
    QFileInfoList toReturn;
    for(QFileInfo file : location.entryInfoList()) {
        if(QRegExp("cell_\\d+_\\d+_dt\\d+_dvars|dt\\d+_dvars").exactMatch(file.baseName())) {
            toReturn << file;
        }
    }
    return toReturn;
}
QFileInfoList Dialog::getFileNamesBar(QDir location, int trial) {
    QFileInfoList toReturn;
    for(QFileInfo file : location.entryInfoList()) {
        if(QRegExp(QString("dss")+QString::number(trial)+QString("_.+")).exactMatch(file.baseName())
            && !QRegExp("dss\\d+_state").exactMatch(file.baseName())
            && !QRegExp("dss\\d+_pvars").exactMatch(file.baseName())){
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
    QString xName = "t";
    if(files.isEmpty()) {
        QMessageBox::warning(0,"error", "No Files Found");
        throw badFile();
    }
    QMap<QString, lineGraph*> graphMap;
    int progressCounter = 0;
    if(files.length() > 10) {
        LoadingProgressDialog* loadingOptions = new LoadingProgressDialog(files, this);
        if(QDialog::Accepted == loadingOptions->exec()) {
            files = loadingOptions->getFilesToLoad();
        }
    }
    QProgressDialog* progressDisp = new QProgressDialog("Opening Files", "Skip", 0, files.size(), this);
    progressDisp->setRange(0,files.size());
    progressDisp->setValue(0);
    progressDisp->show();
    for(QFileInfo fileInfo : files) {
        progressDisp->setValue(progressCounter);
        if(progressDisp->wasCanceled()) {
            break;
        }
        QFile file(fileInfo.absoluteFilePath());
        if(!file.open(QIODevice::ReadOnly)){
            QMessageBox::warning(0,"error", "File " +fileInfo.fileName() + " Could not be Opened");
            continue;
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
        xIndex = split_line.indexOf(xName);
        if(xIndex == -1) {
            QMessageBox::warning(0,"ERROR NO TIME", "File "+fileInfo.fileName()+" had no variable for "+ xName+" which is the selected x variable");
            continue;
        }
        for(int i = 0; i < split_line.length();i++) {
            if(i == xIndex) {
                continue;
            }
            if(!graphMap.contains(split_line[i])) {
                auto graph = new lineGraph(split_line[xIndex],split_line[i],this->read_location);
                graphMap.insert(split_line[i],graph);
                ui->tabWidget->addTab(graph,split_line[i]);
            }
            graphMap[split_line[i]]->addData(y[xIndex],y[i],this->getName(fileInfo));
        }
        progressCounter++;
    }
    progressDisp->deleteLater();
}
void Dialog::buildBarGraphs(int trial) {
    QFileInfoList fileInfos = this->getFileNamesBar(this->read_location, trial);
    for(auto fileInfo : fileInfos) {
        QFile file(fileInfo.absoluteFilePath());
        if(!file.open(QIODevice::ReadOnly)){
            continue;
        }
        QTextStream in(&file);
        QStringList names = in.readLine().split("\t", QString::SkipEmptyParts);
        QStringList values = in.readLine().split("\t", QString::SkipEmptyParts);
        auto name = names.begin();
        auto value = values.begin();
        auto var = fileInfo.baseName().split("_").last();
        for(;name != names.end()&& value != values.end(); name++,value++) {
            ui->tabWidget->addTab(new barGraph(*name, value->toDouble(), var, read_location), var +":"+QString(*name));
        }
    }
}
void Dialog::on_loadNew_clicked()
{
    QFileDialog getDirOrFile(0,tr("Open File"));
    getDirOrFile.setFileMode(QFileDialog::Directory);
    QDir loc = this->read_location;
    loc.cdUp();
    getDirOrFile.setDirectory(loc);
    getDirOrFile.setNameFilter("Directories");//;; All Files (*.*);;Data File (*.txt *.dat)");
    QStringList filenames;
    if(getDirOrFile.exec()) {
        filenames = getDirOrFile.selectedFiles();
    }
    if(filenames.isEmpty()) return;
    while(ui->tabWidget->count() > 0) {
        ui->tabWidget->removeTab(0);
    }
    QFileInfoList fileInfos;
    for(auto filename : filenames) {
        QFileInfo file(filename);
        if(file.isDir()) {
            fileInfos.append(this->getFileNames(QDir(file.absoluteFilePath())));
        } else {
            fileInfos.append(file);
        }
    }
    try {
        buildLineGraphs(fileInfos);
    } catch(badFile e) {
        QMessageBox::warning(0,tr("Error"),tr("Selected files could not be used"));
    }
}

