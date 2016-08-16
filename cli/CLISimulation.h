#ifndef CLI_SIMULATION_H
#define CLI_SIMULATION_H

#include <QObject>
#include "protocol.h"
#include <QVector>
#include <QFuture>
#include <QFutureWatcher>

class CLISimulation : QObject {
    Q_OBJECT
  public:
    CLISimulation(int protoType = -1, QString simvarFile = "", QString dvarFile = "", QString measFile = "", QString pvarFile = "");
    ~CLISimulation() {};
    bool runTrial(int trialnum);
    void runSim();
  private:
    Protocol* proto;
    int low = 0;
    int high = 0;
    QVector<Protocol*> vector;
    QFutureWatcher<void> watcher;
    QFuture<void> next;
  private slots:
    void finish();
    void setRange(int low, int high);
    void setValue(int value);
};

#endif
