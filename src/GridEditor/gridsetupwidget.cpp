#include "gridsetupwidget.h"
#include "cellutils.h"
#include "gridDelegate.h"
#include "guiUtils.h"
#include "inexcitablecell.h"
#include "ui_gridsetupwidget.h"

#include <QHBoxLayout>
#include <QHeaderView>
#include <QLabel>
#include <QPainter>
#include <QPen>
#include <QStandardPaths>
#include <memory>
using namespace std;
using namespace LongQt;

//##############################
// gridSetupWidget
//##############################
GridSetupWidget::GridSetupWidget(QWidget* parent)
    : GridSetupWidget(shared_ptr<GridProtocol>(new GridProtocol()), parent) {}

GridSetupWidget::GridSetupWidget(shared_ptr<GridProtocol> initial_proto,
                                 QWidget* parent)
    : QWidget(parent), ui(new Ui::GridSetupWidget) {
  ui->setupUi(this);
  this->proto = initial_proto;
  this->grid = static_pointer_cast<GridCell>(proto->cell())->getGrid();
  this->model = new GridModel(this->proto, this);

  this->createMenu();
}
GridSetupWidget::~GridSetupWidget() { delete this->ui; }

void GridSetupWidget::createMenu() {
  // setup view
  this->ui->cellGrid->setModel(this->model);
  ui->cellGrid->setItemDelegate(new GridDelegate(this->ui->cellGrid));
  ui->cellGrid->horizontalHeader()->setSectionResizeMode(
      QHeaderView::ResizeToContents);
  auto cellMap = CellUtils::cellMap;
  cellMap["Inexcitable Cell"] = []() { return make_shared<InexcitableCell>(); };
  for (auto it : cellMap) {
    ui->chooseType->addItem(it.first.c_str());
  }
  connect(ui->addColumnButton, &QPushButton::clicked, [this]() {
    this->model->insertColumns(this->model->columnCount(),
                               this->ui->columnInt->value());
  });
  connect(ui->removeColumnButton, &QPushButton::clicked, [this]() {
    int num = this->ui->columnInt->value();
    this->model->removeColumns(this->model->columnCount() - num, num);
  });
  connect(ui->addRowButton, &QPushButton::clicked, [this]() {
    this->model->insertRows(this->model->rowCount(), ui->rowInt->value());
  });
  connect(ui->removeRowButton, &QPushButton::clicked, [this]() {
    int num = ui->rowInt->value();
    this->model->removeRows(this->model->rowCount() - num, num);
  });
  connect(ui->cellGrid->selectionModel(),
          &QItemSelectionModel::selectionChanged,
          [this](const QItemSelection& selected, const QItemSelection&) {
            if (!selected.isEmpty()) {
              bool oldState = this->ui->chooseType->blockSignals(true);
              this->ui->chooseType->setCurrentText(
                  selected.first().topLeft().data().toString());
              this->ui->chooseType->blockSignals(oldState);
            }
          });
  connect(this->model, &GridModel::cellChanged, this,
          &GridSetupWidget::cellChanged);
}

void GridSetupWidget::on_setType_clicked() {
  QString type = this->ui->chooseType->currentText();
  auto selected = this->ui->cellGrid->selectionModel()->selectedIndexes();
  for (auto index : selected) {
    this->model->setData(index, type);
  }
}

void GridSetupWidget::setGrid(Grid* grid) {
  this->grid = grid;
  //	delete this->model;
  this->model = new GridModel(this->proto);
  this->ui->cellGrid->setModel(this->model);
}
Grid* GridSetupWidget::getGrid() { return this->grid; }
shared_ptr<GridProtocol> GridSetupWidget::getProtocol() { return this->proto; }
void GridSetupWidget::on_toggleMeasure_clicked() {
  auto selected = ui->cellGrid->selectionModel()->selectedIndexes();
  for (auto index : selected) {
    this->model->setData(index.child(0, 1), !index.child(0, 1).data().toBool());
  }
}
void GridSetupWidget::on_toggleStim_clicked() {
  auto selected = ui->cellGrid->selectionModel()->selectedIndexes();
  for (auto index : selected) {
    this->model->setData(index.child(0, 0), !index.child(0, 0).data().toBool());
  }
}
QTableView* GridSetupWidget::view() { return this->ui->cellGrid; }
GridModel* GridSetupWidget::getModel() { return this->model; }
void GridSetupWidget::reset() { this->model->reloadModel(); }
