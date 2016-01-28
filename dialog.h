#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>

namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = 0);
    ~Dialog();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_5_clicked();

    void on_pushButton_6_clicked();

    void on_pushButton_7_clicked();

    void on_pushButton_8_clicked();

    void on_pushButton_9_clicked();

    void on_pushButton_10_clicked();

    void on_pushButton_11_clicked();

    bool control_on_graph(QVector<double> &x, QVector<double> &y, QString toGet, QString time);

private:
    Ui::Dialog *ui;
};

#endif // DIALOG_H

/*
#ifndef DIALOG_H
#define DIALOG_H

#include <QDebug>
#include <QProgressDialog>
#include <QDialog>
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
*/
