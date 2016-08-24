#ifndef LOADINGPROGRESS_H
#define LOADINGPROGRESS_H

#include <QDialog>
#include <QFileInfoList>
#include <QCheckBox>

namespace Ui {
class LoadingProgressDialog;
}

class LoadingProgressDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LoadingProgressDialog(QFileInfoList files, QWidget *parent = 0);
    ~LoadingProgressDialog();
    QFileInfoList getFilesToLoad();

private:
    Ui::LoadingProgressDialog *ui;
    QList<QCheckBox*> trials;
    QFileInfoList filesToLoad;
    QFileInfoList files;
    void createOptions();
    void on_continueButton_clicked();
    void on_skipButton_clicked();
};

#endif // LOADINGPROGRESSDIALOG_H
