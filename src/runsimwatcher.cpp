#include "runsimwatcher.h"
#include <QDebug>
#include <QEvent>
#include <chrono>
#define RANGE 100000

RunSimWatcher::RunSimWatcher(LongQt::RunSim *runsim, QObject *parent)
    : QObject(parent), __runsim(runsim), __range(0,0) {
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
  if(this->__range.first == this->__range.second) {
      this->__range = __runsim->progressRange();
  }
  double range = this->__range.second - this->__range.first;
  double progress = (RANGE*__runsim->progress())/range;
  emit progressValueChanged((int)progress);
}

void RunSimWatcher::cancel() { this->__runsim->cancel(); }

void RunSimWatcher::startWatch() {
  using namespace std::chrono;
  QObject::startTimer(milliseconds(500), Qt::CoarseTimer);
  this->__range = __runsim->progressRange();
  emit progressRangeChanged((int)0,(int)RANGE);
}

void RunSimWatcher::finishWatch() { emit finished(); }

#undef RANGE
