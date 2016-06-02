#ifndef BARGRAPH_H
#define BARGRAPH_H

#include <QWidget>
#include <QDir>

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
    void Initialize(QString name, QString var);
    Ui::barGraph *ui;
    QDir saveDir;
    QVector<double> data;
};

#endif // BARGRAPH_H
