#include "choosegraphstoggle.h"
#include "ui_choosegraphstoggle.h"

ChooseGraphsToggle::ChooseGraphsToggle(QCPGraph* graph, QWidget* parent)
    : QWidget(parent), ui(new Ui::ChooseGraphsToggle) {
  ui->setupUi(this);
  this->graph = graph;
  ui->toggleGraph->setChecked(graph->visible());
  ui->toggleGraph->setText(graph->name());
}

ChooseGraphsToggle::~ChooseGraphsToggle() { delete ui; }

bool ChooseGraphsToggle::state() { return graph->visible(); }

void ChooseGraphsToggle::changeState(bool state) {
  ui->toggleGraph->setChecked(state);
  graph->setVisible(state);
  if (state != graph->visible()) emit stateChanged(state);
}
void ChooseGraphsToggle::on_toggleGraph_toggled(bool checked) {
  graph->setVisible(checked);
  emit stateChanged(checked);
}
