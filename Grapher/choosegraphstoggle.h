/*
 * This is the checkbox widget for choosegrpahs
 * it acctually intreacts with the graph to toggle its visiablity
 */
#ifndef CHOOSEGRAPHSTOGGLE_H
#define CHOOSEGRAPHSTOGGLE_H

#include <QWidget>
#include "qcustomplot.h"

namespace Ui {
class ChooseGraphsToggle;
}

class ChooseGraphsToggle : public QWidget
{
    Q_OBJECT

public:
    explicit ChooseGraphsToggle(QCPGraph* graph, QWidget *parent = 0);
    ~ChooseGraphsToggle();
signals:
    void stateChanged(bool state);
private:
    Ui::ChooseGraphsToggle *ui;
    QCPGraph* graph;
private slots:
    void on_toggleGraph_toggled(bool checked);
};

#endif // CHOOSEGRAPHSTOGGLE_H
