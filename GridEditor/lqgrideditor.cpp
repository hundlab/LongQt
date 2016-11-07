#include "lqgrideditor.h"
#include "ui_lqgrideditor.h"

LQGridEditor::LQGridEditor(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::LQGridEditor)
{
    ui->setupUi(this);
}

LQGridEditor::~LQGridEditor()
{
    delete ui;
}
