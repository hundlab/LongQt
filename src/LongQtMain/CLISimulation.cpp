#include "CLISimulation.h"
#include <QDir>
#include <QTextStream>
#include <QtConcurrent>
#include "currentClampProtocol.h"
#include "gridProtocol.h"
#include "settingsIO.h"
#include "voltageClampProtocol.h"
using namespace std;
using namespace LongQt;

CLISimulation::CLISimulation() {
  this->proto.reset(new CurrentClamp());
  connect(&watcher, SIGNAL(finished()), this, SLOT(finish()));
  connect(&watcher, SIGNAL(progressRangeChanged(int, int)), this,
          SLOT(setRange(int, int)));
  connect(&watcher, SIGNAL(progressValueChanged(int)), this,
          SLOT(setValue(int)));
}
CLISimulation::~CLISimulation() {}

void CLISimulation::runSims(QStringList simvarFiles) {
  runner.clear();
  SettingsIO* settingsMgr = SettingsIO::getInstance();
  for (QString& simvarFile : simvarFiles) {
    proto = settingsMgr->readSettings(simvarFile, proto);
    proto->setDataDir();
    runner.appendSims(proto);
  }
  runner.run();
  watcher.setFuture(runner.getFuture());
  QTextStream(stdout) << "Running Simulation...\n";
}
void CLISimulation::finish() {
  QTextStream(stdout) << "\nFinished!\n";
  this->runner.clear();
  exit(0);
}
void CLISimulation::setRange(int low, int high) {
  this->low = low;
  this->high = high;
}
void CLISimulation::setValue(int value) {
  int denom = high - low;
  if (denom == 0) {
    return;
  }
  int percent = (100 * (value - this->low)) / denom;
  QTextStream(stdout) << "%" << percent << " ";
}
