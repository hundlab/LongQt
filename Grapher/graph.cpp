#include "graph.h"
#include "linegraph.h"
#include "bargraph.h"
#include "ui_graph.h"
#include "measure.h"
#include "loadingprogress.h"

#include <QIODevice>
#include <QList>
#include <QGridLayout>
#include <QMap>
#include <QProgressDialog>
#include <exception>
#include <QPointer>
#include <QScopedPointer>

Grapher::Grapher(QDir read_location, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Grapher)
{
    ui->setupUi(this);
    this->read_location = read_location;
    this->Initialize();
}
void Grapher::Initialize() {
    auto dssData = this->dssData();
    auto fileNames = this->getFileNames();
    this->buildLineGraphs(fileNames,dssData);
    this->buildBarGraphs(dssData);
    this->setWindowTitle(this->read_location.dirName());
}
QFileInfoList Grapher::getFileNames() {
    return this->getFileNames(this->read_location);
}
QFileInfoList Grapher::getFileNames(QDir location) {
    QFileInfoList toReturn;
    for(QFileInfo file : location.entryInfoList()) {
        if(QRegExp("cell_\\d+_\\d+_dt\\d+_dvars|dt\\d+_dvars").exactMatch(file.baseName())) {
            toReturn << file;
        }
    }
    return toReturn;
}
QFileInfoList Grapher::getFileNamesBar(QDir location) {
    QFileInfoList toReturn;
    for(QFileInfo file: location.entryInfoList(QDir::Files)) {
        if(QRegExp("\\S*dss\\d+").exactMatch(file.baseName())
            && !QRegExp("dss\\d+_state").exactMatch(file.baseName())
            && !QRegExp("dss\\d+_pvars").exactMatch(file.baseName())){
            toReturn << file;
        }
    }
    return toReturn;
}
QString Grapher::getName(QFileInfo file) {
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
Grapher::~Grapher()
{
    delete ui;
}
void Grapher::buildLineGraphs(QFileInfoList files, DssD dssData){
    QString xName = "t";
    if(files.isEmpty()) {
        QMessageBox::warning(0,"error", "No Files Found");
        throw BadFile();
    }
    QMap<QString, LineGraph*> graphMap;
    int progressCounter = 0;
    QScopedPointer<LoadingProgressDialog> loadingOptions(new LoadingProgressDialog(files, this));
    if(files.length() > 10) {
        if(QDialog::Accepted == loadingOptions->exec()) {
            files = loadingOptions->getFilesToLoad();
        } else {
            files.clear();
        }
        }
    QScopedPointer<QProgressDialog> progressDisp(new QProgressDialog("Opening Files", "Skip", 0, files.size(), this));
    progressDisp->setValue(0);
    progressDisp->show();

    for(QFileInfo fileInfo : files) {
        progressDisp->setValue(progressCounter);
        QCoreApplication::processEvents();
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
                auto graph = new LineGraph(split_line[xIndex],split_line[i],this->read_location);
                graphMap.insert(split_line[i],graph);
                ui->tabWidget->addTab(graph,split_line[i]);
                graph->populateList(dssData);
            }
            graphMap[split_line[i]]->addData(y[xIndex],y[i],this->getName(fileInfo));
        }
        progressCounter++;
    }
}
Grapher::DssD Grapher::dssData() {
    QFileInfoList fileInfos = this->getFileNamesBar(this->read_location);
    DssD dssDat;

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
        for(;name != names.end()&& value != values.end(); name++,value++) {
            auto splitName = name->split("/");
            if(splitName.size() < 2) continue;
            auto var = splitName[splitName.size()-2];
            auto property = splitName.last();
            auto instance =
            splitName.size() > 2?
                splitName[splitName.size()-3]
                :"trail"+fileInfo.baseName().split('s').last();
            dssDat.append(std::make_tuple(instance,var,property,value->toDouble()));
        }
    }
    return dssDat;
}
void Grapher::buildBarGraphs(DssD dssData) {
    QMap<QString,barGraph*> bars;
    for(auto value: dssData) {
        auto instance = get<0>(value);
        auto var = get<1>(value);
        auto property = get<2>(value);
        if(!bars.contains(var+":"+property)) {
             auto bar = new barGraph(instance, get<3>(value), var, read_location);
             ui->tabWidget->addTab(bar, var +":"+property);
             bars.insert(var+":"+property,bar);
         } else {
             bars[var+":"+property]->addBar(instance,get<3>(value));
         }
    }
}

void Grapher::on_loadNew_clicked()
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
        this->setWindowTitle(file.fileName());
        if(file.isDir()) {
            fileInfos.append(this->getFileNames(QDir(file.absoluteFilePath())));
        } else {
            fileInfos.append(file);
        }
    }
    try {
        buildLineGraphs(fileInfos);
    } catch(BadFile e) {
        QMessageBox::warning(0,tr("Error"),tr("Selected files could not be used"));
    }
}

