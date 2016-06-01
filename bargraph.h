#ifndef BARGRAPH_H
#define BARGRAPH_H

#include <QWidget>

namespace Ui {
class barGraph;
}

class barGraph : public QWidget
{
    Q_OBJECT

public:
    explicit barGraph(QWidget *parent = 0);
    ~barGraph();

private:
    Ui::barGraph *ui;
};

#endif // BARGRAPH_H
