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
  std::vector<std::pair<std::string, int>> optList(optMap.begin(),
                                                   optMap.end());
  std::sort(optList.begin(), optList.end(),
            [](const auto& a, const auto& b) { return a.second < b.second; });
  for (auto& opt : optList) {
    std::string name = opt.first;
    auto cbox = new QCheckBox(name.c_str());
    vbox->addWidget(cbox);
    checkMap[name] = cbox;
    connect(cbox, &QCheckBox::clicked,
            [this, name]() { this->update_model(name); });
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

void SimvCellOpts::update_model(string name) {
  string value = name + "|";
  for (const string& checkbox : this->checkMap.keys()) {
    if (checkMap[checkbox]->checkState() && name != checkbox) {
      value += checkbox + "|";
    }
  }
  if (name == "WT") {
    value = "WT";
  }
  auto conflicts = proto->cell()->checkConflicts(CellUtils::strToFlag(value));
  proto->parsStr(this->name, value);
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
