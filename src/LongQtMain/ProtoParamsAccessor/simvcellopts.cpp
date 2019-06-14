#include "simvcellopts.h"
#include "oneitemlayout.h"

#include <protocol.h>
#include <QCheckBox>
#include <QLineEdit>
#include <QVBoxLayout>
#include <algorithm>
#include <list>
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
  auto optMap = this->proto->cell()->optionsMap();
  for (auto& opt : optMap) {
    std::string name = opt.first;
    auto cbox = new QCheckBox(name.c_str());
    vbox->addWidget(cbox);
    checkMap[name] = cbox;
    cbox->setToolTip(this->proto->cell()->optionDesc(name).c_str());
    connect(cbox, &QCheckBox::clicked,
            [this, name](bool value) { this->update_model(name, value); });
  }
  this->update_ui();
}

void SimvCellOpts::update_ui() {
  for (auto& opt : this->proto->cell()->optionsMap()) {
    this->checkMap[opt.first.c_str()]->setChecked(opt.second);
  }
  emit updated();
}

void SimvCellOpts::update_model(string name, bool value) {
  this->proto->cell()->setOption(name, value);
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
