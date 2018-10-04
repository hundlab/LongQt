/*
 * Class contains one bar graph (multiple bars)
 */
#ifndef BARGRAPH_H
#define BARGRAPH_H

#include <QWidget>
#include <QDir>
#include <QColor>
#include <QMap>
#include "qcustomplot.h"

namespace Ui {
class BarGraph;
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
	// constructs the bar graph with one bar
    explicit barGraph(QString name, double value, QString var, QDir saveDir, QWidget *parent = 0);
    ~barGraph();

    void addBar(QString name, double value);
private:
	// prepares bar graph 
    void Initialize();
	// get new axis ranges
    void setRange(bar newBar);
	// add annother bar
    void newBar(bar& newBar);
	// is the same as in guiUtils
    QColor genColor(int numBars);
    Ui::BarGraph *ui;
    QDir saveDir;
    QVector<QString> labels;
    QVector<double> ticks;
    QString var;
    QString aspect;
    QMap<QString, QString> unitsMap;
private slots:
	// will load a new set of bars as give by the user
    void on_loadOtherTrial_clicked();
	// exports bar graph as jpg
    void on_save_clicked();
};

#endif // BARGRAPH_H
