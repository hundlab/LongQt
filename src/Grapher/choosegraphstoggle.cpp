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
void ChooseGraphsToggle::on_toggleGraph_toggled(bool checked) {
  graph->setVisible(checked);
  emit stateChanged(checked);
}
