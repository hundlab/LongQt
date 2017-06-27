#include <QStringList>
#include <QMessageBox>

#include "ionchannelconfig.h"
#include "ui_ionchannelconfig.h"
#include "pvarsgrid.h"

IonChannelConfig::IonChannelConfig(QTableView* view, GridProtocol* proto, QWidget *parent) :
	QWidget(parent),
	ui(new Ui::IonChannelConfig)
{
	ui->setupUi(this);
	this->view = view;
	this->model = (GridModel*)view->model();
	this->proto = proto;
	this->cellChanged(proto->cell);
	this->updateList();
    ui->listWidget->addAction(ui->actionDelete);
    ui->listWidget->addAction(ui->actionShow_Cells);
	connect(this->model, &GridModel::cellChanged, this, &IonChannelConfig::cellChanged);
}

IonChannelConfig::~IonChannelConfig()
{
	delete ui;
}

void IonChannelConfig::updateList() {
	QStringList toAdd;
	this->ui->listWidget->clear();
	for(auto& pvar : *this->proto->pvars) {
		toAdd += pvar.second->IonChanParam::str(pvar.first).c_str();
	}
	this->ui->listWidget->addItems(toAdd);
}

void IonChannelConfig::cellChanged(Cell* cell) {
	if(cell != proto->cell) {
		qWarning("IonChannelConfig: Cell does not match proto cell");
	}
	QRegExp allowed_vars = QRegExp("Factor");
	QStringList toAdd;
	ui->ionChannelType->clear();
	for(auto& pvarName : this->proto->cell->getConstants()) {
		if(allowed_vars.indexIn(pvarName.c_str()) != -1) {
			toAdd += pvarName.c_str();
		}
	}
	toAdd.removeDuplicates();
	ui->ionChannelType->addItems(toAdd);
}

void IonChannelConfig::on_randomize_stateChanged(int state) {
	ui->normalDist->setVisible(state);
	ui->lognormalDist->setVisible(state);
	ui->mean->setVisible(state);
	ui->meanLabel->setVisible(state);
	ui->stdDev->setVisible(state);
	ui->stdDevLabel->setVisible(state);
	ui->startVal->setVisible(!state);
	ui->startValLabel->setVisible(!state);
	ui->incAmt->setVisible(!state);
	ui->incAmtLabel->setVisible(!state);
}

void IonChannelConfig::on_multiple_stateChanged(int state) {
	ui->maxDist->setValue(0);
	ui->maxVal->setValue(1000);
	ui->maxDist->setEnabled(state);
	ui->maxDistLabel->setEnabled(state);
	ui->maxVal->setEnabled(state);
	ui->maxValLabel->setEnabled(state);
}

void IonChannelConfig::on_normalDist_toggled(bool checked) {
	if(checked) {
	if(ui->mean->value() == 0) {
		ui->mean->setValue(1);
	}
	} else {
		if(ui->mean->value() == 1) {
			ui->mean->setValue(0);
		}
	}
}

void IonChannelConfig::on_addButton_clicked()
{
	int maxDist = 0;
	double maxVal = 100;
	string type = ui->ionChannelType->currentText().toStdString();
	CellPvars::IonChanParam toAdd = CellPvars::IonChanParam();
	if(ui->randomize->checkState() == 0) {
		toAdd.dist = CellPvars::Distribution::none;
		toAdd.val[0] = ui->startVal->value();
		toAdd.val[1] = ui->incAmt->value();
	} else {
		toAdd.val[0] = ui->mean->value();
		toAdd.val[1] = ui->stdDev->value();
		if(ui->normalDist->isChecked()) {
			toAdd.dist = CellPvars::Distribution::normal;
		} else {
			toAdd.dist = CellPvars::Distribution::lognormal;
		}
	}
	proto->pvars->insert(type,toAdd);
	if(ui->multiple->isChecked()) {
		maxDist = ui->maxDist->value();
		maxVal = ui->maxVal->value();
        ((PvarsGrid*)proto->pvars.get())->setMaxDistAndVal(type,maxDist,maxVal);
	}
	auto startCells = this->getInitial();
    ((PvarsGrid*)proto->pvars.get())->setStartCells(type,startCells);
	this->updateList();
}
set<pair<int,int>> IonChannelConfig::getInitial() {
	set<pair<int,int>> startCells;
	auto selected = this->view->selectionModel()->selectedIndexes();
	for(auto toAdd : selected) {
		startCells.insert({toAdd.column(),toAdd.row()});
	}
	return startCells;
}

void IonChannelConfig::on_actionDelete_triggered() {
    QList<QListWidgetItem *> items = ui->listWidget->selectedItems();
    for(auto item: items) {
        QString name = item->data(Qt::DisplayRole).toString().split("\t")[0];
        this->proto->pvars->erase(name.toStdString());
        int row = ui->listWidget->row(item);
        ui->listWidget->takeItem(row);
        delete item;
    }
}
void IonChannelConfig::on_actionShow_Cells_triggered() {
    QList<QListWidgetItem *> items = ui->listWidget->selectedItems();
    QString text;
    for(auto item: items) {
        QString name = item->data(Qt::DisplayRole).toString().split("\t")[0];
		text += this->proto->pvars->at(name.toStdString())->str(name.toStdString()).c_str();
    }
    QMessageBox msgBox;
    msgBox.setText(text);
    msgBox.exec();
}
