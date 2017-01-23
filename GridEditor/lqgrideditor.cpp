#include "lqgrideditor.h"
#include "ui_lqgrideditor.h"
#include <QStatusBar>
#include <QFileDialog>
#include <QStandardPaths>

LQGridEditor::LQGridEditor(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::LQGridEditor)
{
	ui->setupUi(this);
	this->setStatusBar(new QStatusBar());
	this->gridView = this->ui->centralWidget->view();
}

LQGridEditor::~LQGridEditor()
{
	delete ui;
}
void LQGridEditor::on_actionSet_Conductivities_triggered()
{
	if(this->condEdit == 0) {
		this->condEdit = new ConductivityEditor(this->gridView);
		this->condEdit->show();
		connect(this->condEdit, &QObject::destroyed, [this] () {
			this->condEdit = 0;
		});
	} else {
		this->condEdit->show();
		this->condEdit->raise();
	}
}
void LQGridEditor::on_actionNew_triggered() {
	this->ui->centralWidget->getModel()->clear();
	this->saveFile = "";
}
void LQGridEditor::on_actionOpen_triggered() {
    QString fileName = QFileDialog::getOpenFileName(this,"Open Grid File",QStandardPaths::standardLocations(QStandardPaths::DocumentsLocation).first());
    if (!fileName.isEmpty()){
        this->ui->centralWidget->getProtocol()->readpars(fileName.toStdString());
    }
	this->ui->centralWidget->getModel()->reloadModel();
}
void LQGridEditor::on_actionSave_triggered() {
	if(this->saveFile == "") {
		this->on_actionSave_As_triggered();
	} else {
		this->ui->centralWidget->getProtocol()->writepars(this->saveFile.toStdString(),2);
	}
}
void LQGridEditor::on_actionSave_As_triggered() {
    QString fileName = QFileDialog::getSaveFileName(this);
    if (!fileName.isEmpty()){
		this->saveFile = fileName;
	    this->ui->centralWidget->getProtocol()->writepars(fileName.toStdString(),2);
    }
}
