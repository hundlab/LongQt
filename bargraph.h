#ifndef BARGRAPH_H
#define BARGRAPH_H

#include <QWidget>
#include <QDir>
#include "qcustomplot.h"

namespace Ui {
class barGraph;
}

class barGraph : public QWidget
{
    Q_OBJECT
public:
    explicit barGraph(QString name, double value, QString var, QDir saveDir, QWidget *parent = 0);
    ~barGraph();
private:
    void Initialize();
    void newBar();
    Ui::barGraph *ui;
    QDir saveDir;
    QVector<double> data;
    QVector<QString> labels;
    QVector<double> ticks;
    QString var;
    QCPBars* valueBar;
private slots:
    void on_loadOtherTrial_clicked();
    void on_save_clicked();
};

#endif // BARGRAPH_H
