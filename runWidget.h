#ifndef runWidget_H
#define runWidget_H

#include <QWidget>
#include <string>
#include <QDir>
#include <QPushButton>
#include <QProgressBar>
#include <QFutureWatcher>
#include <QFuture>

#include "protocol.h"
#include "menu_object.h"

using namespace std;

class runWidget : public QWidget {
Q_OBJECT
  public:
    runWidget(Protocol* proto, QDir working_dir, QWidget* parent = 0);
    void setWorkingDir(QDir dir);
    void setProto(Protocol* proto);
  public slots:
    void cancel();
  signals:
    void canceled();
    void finished();
    void running();
  private slots:
    void run_sims();
    void finish();
  private:
    QWidget* parent;
    Protocol* proto;
    QPushButton* run_button;
    QPushButton* cancel_button;
    QProgressBar* pdialog;
    QDir working_dir;
    QFutureWatcher<void> watcher;
    QFuture<void> next;
    QVector<Protocol*> vector;
};
//need to reset Vector and reenable run_button
#endif
