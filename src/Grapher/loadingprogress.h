/*
 * for large sims it can take a while to load
 * this allows for someone to see the progress and select which parts should be 
 * graphed
 *
 */
#ifndef LOADINGPROGRESS_H
#define LOADINGPROGRESS_H

#include <QDialog>
#include <QFileInfoList>
#include <QCheckBox>
#include <QMap>

namespace Ui {
class LoadingProgressDialog;
}

class LoadingProgressDialog : public QDialog
{
    Q_OBJECT
    Q_PROPERTY(bool multiCell READ multiCell WRITE setMultiCell)

public:
    explicit LoadingProgressDialog(QFileInfoList files, QWidget *parent = 0);
    ~LoadingProgressDialog();
    QFileInfoList getFilesToLoad() const;
    bool multiCell();
    void setMultiCell(bool value);

protected slots:
    virtual void closeEvent(QCloseEvent* e);

private:
    Ui::LoadingProgressDialog *ui;
    bool m_multiCell = false;
    QMap<QPair<int,int>,QCheckBox*> trials;
    QFileInfoList filesToLoad;
    QFileInfoList files;
    void createOptions();
private slots:
    void on_skipButton_clicked();
    void on_loadAll_clicked();
    void on_loadFreqButton_clicked();
};

#endif // LOADINGPROGRESSDIALOG_H
