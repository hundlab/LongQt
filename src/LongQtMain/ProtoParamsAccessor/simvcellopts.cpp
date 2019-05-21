#include "simvcellopts.h"
#include "oneitemlayout.h"

#include <protocol.h>
#include <QCheckBox>
#include <QLineEdit>
#include <QVBoxLayout>
using namespace std;
using namespace LongQt;

SimvCellOpts::SimvCellOpts(shared_ptr<Protocol> proto, string name,
                           QGroupBox* parent)
    : Simvar(proto, name, parent) {
  parent->setFlat(true);
  parent->setTitle(this->getPrettyName());
  parent->setToolTip(this->getToolTip());
  this->vbox = new QVBoxLayout(parent);
  this->setup(parent);
}

void SimvCellOpts::setup(QGroupBox* parent) {
  for (auto& opt : this->proto->cell()->optionsMap()) {
    auto cbox = new QCheckBox(opt.first.c_str());
    vbox->addWidget(cbox);
    checkMap[opt.first] = cbox;
    connect(cbox, &QCheckBox::clicked, this, &SimvCellOpts::update_model);
  }
  this->update_ui();
}

void SimvCellOpts::update_ui() {
  QString model_line = QString(proto->parsStr(this->name).c_str());
  QStringList optsList = model_line.split("|", QString::SkipEmptyParts);
  for (auto& check : checkMap) {
    check->setChecked(false);
  }
  for (auto& opt : optsList) {
    this->checkMap[opt.toStdString()]->setChecked(true);
  }
  emit updated();
}

void SimvCellOpts::update_model(bool) {
  string value;
  for (const string& checkbox : this->checkMap.keys()) {
    if (checkMap[checkbox]->checkState()) {
      value += checkbox + "|";
    }
  }
  proto->parsStr(name, value);
  this->update_ui();
}

void SimvCellOpts::changeCell(shared_ptr<Cell> cell) {
  if (cell != proto->cell()) {
    Logger::getInstance()->write(
        "SimvCellOpts: Protocol cell does not match new cell");
  }
  QLayoutItem* item;
  while ((item = vbox->takeAt(0)) != 0) {
    if (item) {
      delete item->widget();
    }
    delete item;
  }
  this->checkMap.clear();
  auto parent = static_cast<QGroupBox*>(this->parent());
  this->setup(parent);
}

SimvCellOpts::~SimvCellOpts() {}
