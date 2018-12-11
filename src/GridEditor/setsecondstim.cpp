#include "setsecondstim.h"
#include "ui_setsecondstim.h"

SetSecondStim::SetSecondStim(QWidget *parent)
    : QWidget(parent), ui(new Ui::SetSecondStim) {
  ui->setupUi(this);
}

SetSecondStim::~SetSecondStim() { delete ui; }
