#ifndef CLI_SIMULATION_H
#define CLI_SIMULATION_H

#include <QFutureWatcher>
#include <QObject>
#include "protocol.h"
#include "runsim.h"
#include "runsimwatcher.h"
namespace LQ = LongQt;

class CLISimulation : QObject {
  Q_OBJECT
 public:
  CLISimulation(QObject* parent = nullptr);
  ~CLISimulation();
  void runSims(QStringList simvarFiles);

 private:
  std::shared_ptr<LQ::Protocol> proto = 0;
  int low = 0;
  int high = 0;
  LQ::RunSim runner;
  RunSimWatcher watcher;
 private slots:
  void finish();
  void setRange(int low, int high);
  void setValue(int value);
};

#endif
