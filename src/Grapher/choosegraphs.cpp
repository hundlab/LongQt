#include "choosegraphs.h"
#include "choosegraphstoggle.h"
#include "ui_choosegraphs.h"

ChooseGraphs::ChooseGraphs(QCustomPlot* plot, QWidget* parent)
    : QDialog(parent), ui(new Ui::ChooseGraphs) {
  ui->setupUi(this);
  this->plot = plot;
  int numCollums = 5;
  for (int i = 0; i < plot->graphCount(); i++) {
    QCPGraph* graph = plot->graph(i);
    if (graph == NULL) {
      continue;
    }
    ChooseGraphsToggle* chooseToggle = new ChooseGraphsToggle(graph);
    ui->toggleButtonLayout->addWidget(chooseToggle, i / numCollums,
                                      i % numCollums);
    connect(chooseToggle, &ChooseGraphsToggle::stateChanged,
            [=](bool) { plot->replot(); });
    connect(ui->toggleButton, &QPushButton::pressed,
            [=]() { chooseToggle->changeState(!chooseToggle->state()); });
  }
}

ChooseGraphs::~ChooseGraphs() { delete ui; }
