#include "bargraph.h"
#include "ui_bargraph.h"

barGraph::barGraph(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::barGraph)
{
    ui->setupUi(this);
}

barGraph::~barGraph()
{
    delete ui;
}
