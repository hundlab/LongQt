#ifndef RUNSIMWATCHER_H
#define RUNSIMWATCHER_H

#include <QObject>
#include "runsim.h"
namespace LQ = LongQt;

class RunSimWatcher : public QObject {
  Q_OBJECT
 public:
  explicit RunSimWatcher(LQ::RunSim* runsim, QObject* parent = nullptr);

  virtual void timerEvent(QTimerEvent* e) override;
  void cancel();

 signals:
  void progressRangeChanged(int, int);
  void progressValueChanged(int);
  void finished();

 private:
  void startWatch();
  void finishWatch();

  LQ::RunSim* __runsim;
  bool __started = false;
};

#endif  // RUNSIMWATCHER_H
