#include "simvfile.h"
#include "oneitemlayout.h"

#include <protocol.h>
using namespace std;
using namespace LongQt;

SimvFile::SimvFile(shared_ptr<Protocol> proto, string name, QWidget *parent)
    : Simvar(proto, name, parent) {
  this->widg = new QLineEdit(this);
  auto layout = new OneItemLayout(this);
  layout->addWidget(widg);
  widg->setReadOnly(true);
  connect(widg, &QLineEdit::textEdited, this, &SimvFile::update_model);
}

void SimvFile::update_ui() {
  QString model_line = QString(proto->parsStr(this->name).c_str());
  if (widg->text() != model_line) {
    widg->setText(model_line);
  }
  emit updated();
}

void SimvFile::update_model(QString value) {
  proto->parsStr(name, value.toStdString());
}

SimvFile::~SimvFile() {}
