/*
 * The widget which allows for notes to be taken and displays the progress of
 * the simulation
 */
#ifndef runWidget_H
#define runWidget_H

#include <QDir>
#include <QLineEdit>
#include <QProgressBar>
#include <QPushButton>
#include <QSharedPointer>
#include <QTextEdit>
#include <QWidget>
#include <string>

#include "protocol.h"
#include "runsim.h"
#include "runsimwatcher.h"
namespace LQ = LongQt;

namespace Ui {
class RunWidget;
}
// at the moment the progress bar is not terribly helpful this is because in the
// multithreading done with QtConncurrent there is no good way to report
// progress from inside one simulation only multiple trials can be displayed
class RunWidget : public QWidget {
  Q_OBJECT
 public:
  RunWidget(std::shared_ptr<LQ::Protocol> proto, QDir working_dir,
            QWidget* parent = 0);
 public slots:
  void setWorkingDir(QDir& dir);
  void setProto(std::shared_ptr<LQ::Protocol> proto);
  void cancel();
 signals:
  void canceled();
  void finished();
  void running();
 private slots:
  void on_runButton_clicked();
  void on_saveButton_clicked();
  void finish();

 private:
  void write_note();

  Ui::RunWidget* ui;
  QWidget* parent;
  std::shared_ptr<LQ::Protocol> proto;
  QDir working_dir;
  LQ::RunSim runner;
  RunSimWatcher watcher;
};
#endif
