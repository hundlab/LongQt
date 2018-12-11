#include "simvcellopts.h"
#include "oneitemlayout.h"

#include <protocol.h>
#include <QCheckBox>
#include <QLineEdit>
#include <QVBoxLayout>
using namespace std;
using namespace LongQt;

SimvCellOpts::SimvCellOpts(shared_ptr<Protocol> proto, string name,
                           QWidget* parent)
    : Simvar(proto, name, parent) {
  this->createMenu();
}

void SimvCellOpts::createMenu() {
  this->widg = new QGroupBox(this);
  auto layout = new OneItemLayout(this);
  layout->addWidget(widg);
  auto vbox = new QVBoxLayout(widg);
  bool first = true;
  for (auto& opt : this->proto->cell()->optionsMap()) {
    auto cbox = new QCheckBox(opt.first.c_str());
    vbox->addWidget(cbox);
    checkMap[opt.first] = cbox;
    connect(cbox, &QCheckBox::clicked, this, &SimvCellOpts::update_model);
    if (first) {
      first = false;
    }
  }
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
  std::for_each(this->checkMap.keyBegin(), this->checkMap.keyEnd(),
                [this, &value](const string& checkbox) {
                  if (checkMap[checkbox]->checkState()) {
                    value += checkbox + "|";
                  }
                });
  proto->parsStr(name, value);
  this->update_ui();
}

void SimvCellOpts::changeCell(shared_ptr<Cell>) {
  qDeleteAll(this->children());
  this->checkMap.clear();
  this->createMenu();
}

SimvCellOpts::~SimvCellOpts() {}
