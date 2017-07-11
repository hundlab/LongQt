#include "lqgrideditor.h"
#include "ui_lqgrideditor.h"
#include "simvarmenu.h"
#include "settingsIO.h"

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
	this->proto = this->ui->centralWidget->getProtocol();
	SettingsIO::getInstance()->allowProtoChange = false;
}

LQGridEditor::~LQGridEditor()
{
	delete ui;
}
void LQGridEditor::on_actionNew_triggered() {
	this->ui->centralWidget->getModel()->clear();
	this->saveFile = "";
}
void LQGridEditor::on_actionOpen_triggered() {
    QString fileName = QFileDialog::getOpenFileName(this,"Open Grid File",QStandardPaths::standardLocations(QStandardPaths::DocumentsLocation).first());
    if (!fileName.isEmpty()){
		shared_ptr<Protocol> proto = this->ui->centralWidget->getProtocol();
		SettingsIO::getInstance()->readSettings(proto,fileName);
    }
	this->ui->centralWidget->getModel()->reloadModel();
}
void LQGridEditor::on_actionSave_triggered() {
	if(this->saveFile == "") {
		this->on_actionSave_As_triggered();
	} else {
		SettingsIO::getInstance()->writeSettings(this->ui->centralWidget->getProtocol(),this->saveFile);
	}
}
void LQGridEditor::on_actionSave_As_triggered() {
    QString fileName = QFileDialog::getSaveFileName(this,"Save As",QStandardPaths::standardLocations(QStandardPaths::DocumentsLocation).first());
    if (!fileName.isEmpty()){
		this->saveFile = fileName;
		shared_ptr<Protocol> proto = this->ui->centralWidget->getProtocol();
		SettingsIO::getInstance()->writeSettings(proto,fileName);
    }
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
void LQGridEditor::on_actionConfigure_Ion_Channels_triggered() {
	if(this->ionConfig == 0) {
		this->ionConfig = new IonChannelConfig(this->gridView, this->proto, this);
		this->ionConfig->show();
		connect(this->ionConfig, &QObject::destroyed, [this] () {
			this->ionConfig = 0;
		});
	} else {
		this->ionConfig->show();
		this->ionConfig->raise();
	}
}

void LQGridEditor::on_actionToggle_Second_Stim_triggered() {
    this->proto->pars["secondStim"].set(CellUtils::to_string(
				!CellUtils::stob(this->proto->pars["secondStim"].get())));
}

void LQGridEditor::on_actionSet_Sim_Parameters_triggered() {
    simvarMenu* menu = new simvarMenu(this->proto);
    menu->removeGrid();
    menu->show();
}
