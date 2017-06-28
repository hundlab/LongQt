#include "CLISimulation.h"
#include <QTextStream>
#include <QtConcurrent>
#include <QDir>
#include "currentClampProtocol.h"
#include "voltageClampProtocol.h"
#include "gridProtocol.h"
#include "settingsIO.h"

CLISimulation::CLISimulation() {
    this->proto = new CurrentClamp();
    connect(&watcher,SIGNAL(finished()),this,SLOT(finish()));
    connect(&watcher,SIGNAL(progressRangeChanged(int,int)),this,SLOT(setRange(int,int)));
    connect(&watcher,SIGNAL(progressValueChanged(int)),this,SLOT(setValue(int)));
}
CLISimulation::~CLISimulation() {
    if(this->proto) {
        delete proto;
    }
}

void CLISimulation::runSims(QStringList simvarFiles) {
    SettingsIO* settingsMgr = SettingsIO::getInstance();
    QString dateTime = QDate::currentDate().toString("MMddyy")+
        "-"+QTime::currentTime().toString("hhmm");
    int i = 0;
    for(QString& simvarFile: simvarFiles) {
        settingsMgr->readSettings(proto, simvarFile);
        if(settingsMgr->lastProto != this->proto) {
            delete proto;
            proto = settingsMgr->lastProto;
        }
        proto->datadir = (QStandardPaths::standardLocations(
            QStandardPaths::DocumentsLocation).first()
            +"/data"+dateTime+"_"+QString::number(i)).toStdString();
        this->runSim();
        i++;
    }
}
bool CLISimulation::runTrial(int trialnum) {
    proto->setTrial(trialnum);
    return proto->runTrial();
}
void CLISimulation::runSim() {
    int i = 0;
    vector.clear();
    QDir().mkpath(proto->datadir.c_str());
    for( i = 0; i < proto->numtrials; i++) {
        proto->setTrial(i);
        /*        proto->readfile = "r"+ to_string(i) + ".dat"; // File to read SV ICs
                  proto->savefile = "s"+ to_string(i) + ".dat"; // File to save final SV
                  proto->propertyoutfile = "dt%d_%s" + string(".dat");
                  proto->dvarsoutfile = "dt%d_dvars" + string(".dat");
                  proto->finalpropertyoutfile = "dss%d_%s" + string(".dat");
                  proto->finaldvarsoutfile = "dss%d_pvars" + string(".dat");*/
        vector.append(QSharedPointer<Protocol>(proto->clone()));
    }
    this->next = QtConcurrent::map(vector,[] (QSharedPointer<Protocol> p) {
            if(p != NULL) {
            p->runTrial();
            }
            });  // pass vector of protocols to QtConcurrent

    watcher.setFuture(next);
    QTextStream(stdout) << "Running Simulation...\n";
}
void CLISimulation::finish() {
    QTextStream(stdout) << "\nFinished!\n";
    this->vector.clear();
    exit(0);
}
void CLISimulation::setRange(int low, int high) {
    this->low = low;
    this->high = high;
}
void CLISimulation::setValue(int value) {
    int denom = high - low;
    if(denom == 0) {return;}
    int percent = (100*(value - this->low))/denom;
    QTextStream(stdout) << "%" << percent << " ";
}
