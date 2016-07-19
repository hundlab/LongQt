#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QListWidget>
#include <QDir>
#include <exception>

#include "qcustomplot.h"

class badFile : public std::exception
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
    explicit Dialog(QDir read_locaiton, QWidget *parent = 0);
    ~Dialog();
private:
    void Initialize();
    QFileInfoList getFileNames(QDir location);
    QFileInfoList getFileNames();
    QFileInfoList getFileNamesBar(QDir location, int trial);
    QString getName(QFileInfo file);
    QDir read_location;
private slots:
    void buildLineGraphs(QFileInfoList files);
    void buildBarGraphs(int trial);
    void on_loadNew_clicked();

private:
    Ui::Dialog *ui;
};

#endif // DIALOG_H
