#ifndef BARGRAPH_H
#define BARGRAPH_H

#include <QWidget>
#include <QDir>
#include <QColor>
#include <QMap>
#include "qcustomplot.h"

namespace Ui {
class barGraph;
}

struct bar
{
    QCPBars* valueBar;
    QVector<double> data;
};

class barGraph : public QWidget
{
    Q_OBJECT
public:
    explicit barGraph(QString name, double value, QString var, QDir saveDir, QWidget *parent = 0);
    ~barGraph();
private:
    void Initialize();
    void setRange(bar newBar);
    void newBar(bar& newBar);
    QColor genColor(int numBars);
    Ui::barGraph *ui;
    QDir saveDir;
    QVector<QString> labels;
    QVector<double> ticks;
    QString var;
    QString aspect;
    QMap<QString, QString> unitsMap;
private slots:
    void on_loadOtherTrial_clicked();
    void on_save_clicked();
};

#endif // BARGRAPH_H
