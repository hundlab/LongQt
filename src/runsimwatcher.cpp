#include "runsimwatcher.h"
#include <QDebug>
#include <QEvent>
#include <chrono>

RunSimWatcher::RunSimWatcher(LongQt::RunSim *runsim, QObject *parent)
    : QObject(parent), __runsim(runsim) {
  __runsim->startCallback(std::bind(&RunSimWatcher::startWatch, this));
  __runsim->finishedCallback(std::bind(&RunSimWatcher::finishWatch, this));
  if (!__runsim->finished()) {
    this->startWatch();
  }
}

void RunSimWatcher::timerEvent(QTimerEvent *e) {
  if (__runsim->finished()) {
    QObject::killTimer(e->timerId());
    return;
  }
  double progress = __runsim->progress();
  emit progressValueChanged(progress);
}

void RunSimWatcher::cancel() { this->__runsim->cancel(); }

void RunSimWatcher::startWatch() {
  using namespace std::chrono;
  QObject::startTimer(milliseconds(500), Qt::CoarseTimer);
  auto range = __runsim->progressRange();
  emit progressRangeChanged((int)range.first, (int)range.second);
}

void RunSimWatcher::finishWatch() { emit finished(); }
