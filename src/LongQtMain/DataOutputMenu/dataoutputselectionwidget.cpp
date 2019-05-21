#include "dataoutputselectionwidget.h"
#include "guiUtils.h"
#include "ui_mvarmenu.h"

#include <logger.h>
#include <measuredefault.h>
#include <QRegExp>
#include <QScopedPointer>
#include <QSpinBox>
#include <QTreeWidget>
using namespace std;
using namespace LongQt;

/*#################################
  begin mvarMenu class
###################################*/
MvarMenu::MvarMenu(shared_ptr<Protocol> proto, QWidget* parent)
    : QWidget(parent), ui(new Ui::MvarMenu) {
  ui->setupUi(this);
  // setup class variables
  this->proto = proto;
  this->model = new DataOutputSelectionModel(this->proto, this);
  ui->measView->setModel(this->model);
  this->beginMeas =
      new SimvDouble(this->proto, "meastime", ui->beginMeasSpinBox);
  this->beginMeas->setupLabel(ui->beginMeasLabel);
  this->beingWrite =
      new SimvDouble(this->proto, "writetime", ui->beginWriteSpinBox);
  this->beingWrite->setupLabel(ui->beginWriteLabel);
  this->writeInt = new SimvInt(this->proto, "writeint", ui->writeIntSpinBox);
  this->writeInt->setupLabel(ui->writeIntLabel);
  this->setupMenu();

  connect(ui->percrepolBox,
          static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),
          [this](int val) { this->proto->measureMgr().percrepol(val); });
}

MvarMenu::~MvarMenu() {}

void MvarMenu::setupMenu() {
  ui->percrepolBox->setValue(proto->measureMgr().percrepol());
  this->beginMeas->update_ui();
  this->beingWrite->update_ui();
  this->writeInt->update_ui();
}
void MvarMenu::reset() {
  bool oldState = ui->measView->blockSignals(true);
  setupMenu();
  ui->measView->blockSignals(oldState);
}
void MvarMenu::changeProto(shared_ptr<Protocol> proto) {
  this->proto = proto;
  this->model->changeProto(proto);
  this->beginMeas->changeProto(this->proto);
  this->beingWrite->changeProto(this->proto);
  this->writeInt->changeProto(this->proto);
  this->reset();
}
void MvarMenu::changeCell(shared_ptr<Cell> cell) {
  if (cell != proto->cell()) {
    Logger::getInstance()->write(
        "DvarMenu: Cell is not the same as proto's cell");
  }
  this->model->changeCell(cell);
  this->beginMeas->changeCell(cell);
  this->beingWrite->changeCell(cell);
  this->writeInt->changeCell(cell);
  this->reset();
}
