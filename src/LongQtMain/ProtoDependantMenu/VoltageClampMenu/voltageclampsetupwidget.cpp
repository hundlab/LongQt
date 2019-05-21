#include "voltageclampsetupwidget.h"
#include "ui_voltageclampsetupwidget.h"
#include "voltageclampdelegate.h"

#include <QDoubleSpinBox>

using namespace LongQt;

VoltageClampSetupWidget::VoltageClampSetupWidget(
    std::shared_ptr<VoltageClamp> initial_proto, QWidget* parent)
    : QWidget(parent), ui(new Ui::VoltageClampSetupWidget) {
  ui->setupUi(this);

  this->proto = initial_proto;
  this->model = new VoltageClampModel(proto, this);
  ui->voltageClampView->setItemDelegate(
      new VoltageClampDelegate(this->ui->voltageClampView));
  this->tMax = new SimvDouble(this->proto, "tMax", ui->tMaxSpinBox);
  this->tMax->setupLabel(ui->tMaxLabel);
  this->createMenu();
  connect(this->model, &QAbstractTableModel::dataChanged, this->tMax,
          &SimvDouble::update_ui);
  //    connect(ui->voltageWidget,&QTableWidget::cellChanged,this,&VoltageClampSetupWidget::update_model);
  //    connect(ui->voltageWidget,&QTableWidget::itemChanged,[this]
  //    (QTableWidgetItem* item)
  //    {this->update_model(item->row(),item->column());});
}

VoltageClampSetupWidget::~VoltageClampSetupWidget() { delete ui; }

void VoltageClampSetupWidget::createMenu() {
  this->ui->voltageClampView->setModel(this->model);
  connect(ui->addClamp, &QPushButton::clicked,
          [this]() { this->model->insertRow(0); });
  connect(ui->removeClamp, &QPushButton::clicked, [this]() {
    auto selected =
        this->ui->voltageClampView->selectionModel()->selectedIndexes();
    std::sort(selected.begin(), selected.end(),
              [](const QModelIndex& idx1, const QModelIndex& idx2) {
                return idx1.row() > idx2.row();
              });
    for (auto index : selected) {
      this->model->removeRow(index.row());
    }
  });
  this->tMax->update_ui();
}
