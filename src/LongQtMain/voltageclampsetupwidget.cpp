#include "voltageclampsetupwidget.h"
#include "ui_voltageclampsetupwidget.h"

VoltageClampSetupWidget::VoltageClampSetupWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::VoltageClampSetupWidget)
{
    ui->setupUi(this);
}

VoltageClampSetupWidget::~VoltageClampSetupWidget()
{
    delete ui;
}
