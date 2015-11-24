#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QProgressDialog>
#include <QDebug>
#include <QtConcurrent>
#include <QThread>
#include <QFutureWatcher>
#include <QMessageBox>
#include <QPointer>

#include "proto.h"

namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = 0);
    ~Dialog();

    static void doTask(Protocol &proto);

private slots:
    void on_pushButton_clicked();

private:
    Ui::Dialog *ui;
};

#endif // DIALOG_H
