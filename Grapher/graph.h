/*
 * the master grapher widget responsible for creating all necessary graphs
 */
#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QListWidget>
#include <QDir>
#include <exception>

#include "qcustomplot.h"

class BadFile : public std::exception
{
  public:
  virtual const char* what() const throw()
  {
    return "File could not be opened";
  }
};

namespace Ui {
class Grapher;
}

class Grapher : public QDialog
{
    Q_OBJECT

public:
	// a directory is specified instead of files so that all
	// the files can be loaded for ex in the case of multiple
	// trials
    explicit Grapher(QDir read_locaiton, QWidget *parent = 0);
    ~Grapher();
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
    Ui::Grapher *ui;
};

#endif // DIALOG_H
