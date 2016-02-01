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
    void on_save1_clicked();
    void on_save2_clicked();
    void on_save3_clicked();
    void on_save4_clicked();
    void on_save5_clicked();
    void on_save6_clicked();
    void on_save7_clicked();
    void on_save8_clicked();
    void on_save9_clicked();
    void on_save10_clicked();
    void on_save11_clicked();
private:
    Ui::Dialog *ui;
};

#endif // DIALOG_H
