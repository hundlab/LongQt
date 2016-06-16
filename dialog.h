#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QListWidget>
#include <QDir>

#include "protocol.h"
#include "qcustomplot.h"
class badFile: public exception
{
  public:
  virtual const char* what() const throw()
  {
    return "File could not be opened";
  }
};

namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(Protocol* inital_proto,QDir read_locaiton, QWidget *parent = 0);
    ~Dialog();
private:
    void Initialize();
    QFileInfoList getFileNames(QDir location);
    QFileInfoList getFileNames();
    QString getName(QFileInfo file);
    QDir read_location;
private slots:
    void buildLineGraphs(QFileInfoList files);
    void buildBarGraphs(int trial);
    void on_loadNew_clicked();

private:
    Ui::Dialog *ui;
    Protocol* proto;
};

#endif // DIALOG_H
