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

class RunWidget : public QWidget {
  Q_OBJECT
 public:
  RunWidget(std::shared_ptr<LQ::Protocol> proto, QWidget* parent = nullptr);
 public slots:
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
  std::shared_ptr<LQ::Protocol> proto;
  LQ::RunSim runner;
  RunSimWatcher watcher;
};
#endif
