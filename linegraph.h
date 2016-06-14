#ifndef LINEGRAPH_H
#define LINEGRAPH_H

#include <QWidget>
#include <QVector>
#include <QPair>
#include <QList>
#include <QDir>

#include "protocol.h"

namespace Ui {
class lineGraph;
}

class lineGraph : public QWidget
{
    Q_OBJECT
public:
    explicit lineGraph(Protocol* proto, QString xLabel, QString yLabel, QDir saveDir, QWidget *parent = 0);
    ~lineGraph();
    void addData(QVector<double>& x, QVector<double>& y, QString name);
private:
    void Initialize();
    void populateList(int trial);

    Ui::lineGraph *ui;
    Protocol* proto;
    QList<QVector<double>> y;
    QList<QVector<double>> x;
    QString xLabel;
    QString yLabel;
    QDir saveDir;
    int controlLocation;
private slots:
    bool control_on_graph(QVector<double> &x, QVector<double> &y);
    void on_save_clicked();
    void on_loadControl_clicked();
};

#endif // LINEGRAPH_H
