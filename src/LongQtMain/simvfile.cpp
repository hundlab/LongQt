#include "simvfile.h"
#include "oneitemlayout.h"

#include <protocol.h>
using namespace std;
using namespace LongQt;

SimvFile::SimvFile(shared_ptr<Protocol> proto, string name, QLineEdit *parent)
    : Simvar(proto, name, parent) {
  parent->setToolTip(this->getToolTip());
  parent->setReadOnly(true);
  connect(parent, &QLineEdit::textEdited, this, &SimvFile::update_model);
}

void SimvFile::update_ui() {
  auto parent = static_cast<QLineEdit *>(this->parent());
  QString model_line = QString(proto->parsStr(this->name).c_str());
  if (parent->text() != model_line) {
    parent->setText(model_line);
  }
  emit updated();
}

void SimvFile::update_model(QString value) {
  proto->parsStr(name, value.toStdString());
}

SimvFile::~SimvFile() {}
