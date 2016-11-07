#include "conductivityeditor.h"
#include "ui_conductivityeditor.h"

ConductivityEditor::ConductivityEditor(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ConductivityEditor)
{
    ui->setupUi(this);
}

ConductivityEditor::~ConductivityEditor()
{
    delete ui;
}
