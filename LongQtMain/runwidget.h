/*
 * The widget which allows for notes to be taken and displays the progress of the
 * simulation
 */
#ifndef runWidget_H
#define runWidget_H

#include <QWidget>
#include <string>
#include <QDir>
#include <QPushButton>
#include <QProgressBar>
#include <QFutureWatcher>
#include <QFuture>
#include <QTextEdit>
#include <QLineEdit>

#include "protocol.h"

using namespace std;
namespace Ui {
class RunWidget;
}
//at the moment the progress bar is not terribly helpful this is because in the
//multithreading done with QtConncurrent there is no good way to report progress
//from inside one simulation only multiple trials can be displayed
class runWidget : public QWidget {
Q_OBJECT
  public:
    runWidget(Protocol* proto, QDir working_dir, QWidget* parent = 0);
  public slots:
    void setWorkingDir(QDir& dir);
    void setProto(Protocol* proto);
    void cancel();
  signals:
    void canceled();
    void finished();
    void running();
  private slots:
    void on_runButton_clicked();
    void finish();
  private:
	Ui::RunWidget *ui;
    QWidget* parent;
    Protocol* proto;
    QDir working_dir;
    QFutureWatcher<void> watcher;
    QFuture<void> next;
    QVector<Protocol*> vector;
};
#endif
