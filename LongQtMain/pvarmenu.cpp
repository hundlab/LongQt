#include "guiUtils.h"
#include "pvarmenu.h"
#include "ui_pvarmenu.h"
#include "cellpvars.h"

#include <QMessageBox>

PvarMenu::PvarMenu(Protocol* proto, QWidget *parent) :
	QWidget(parent),
	ui(new Ui::PvarMenu)
{
	ui->setupUi(this);
	this->proto = proto;
	this->pvarsDescriptions = GuiUtils::readMap(":/hoverText/pvarsDescriptions.txt");
	this->updateIonChannelType();
	this->updateList();
    ui->listWidget->addAction(ui->actionDelete);
    ui->listWidget->addAction(ui->actionShow_Cells);
}

PvarMenu::~PvarMenu()
{
	delete ui;
}

void PvarMenu::updateIonChannelType() {
	QRegExp* allowed_vars = new QRegExp("Factor");
	QStringList toAdd;
	ui->ionChannelType->clear();
	for(auto& pvarName : this->proto->cell->getConstants()) {
		if(allowed_vars->indexIn(pvarName.c_str()) != -1) {
			toAdd += pvarName.c_str();
		}
	}
	toAdd.removeDuplicates();
	ui->ionChannelType->addItems(toAdd);
}

void PvarMenu::updateList() {
	QStringList toAdd;
	this->ui->listWidget->clear();
	for(auto& pvar : *this->proto->pvars) {
		toAdd += pvar.second->IonChanParam::str(pvar.first).c_str();
	}
	this->ui->listWidget->addItems(toAdd);
}
void PvarMenu::changeProto(Protocol* proto) {
	this->proto = proto;
	this->updateList();
}

void PvarMenu::changeCell(Cell*) {
	this->updateIonChannelType();
	this->updateList();
}

void PvarMenu::on_randomize_stateChanged(int state) {
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

void PvarMenu::on_normalDist_toggled(bool checked) {
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
void PvarMenu::on_addButton_clicked()
{
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
	this->proto->pvars->insert(type,toAdd);
	this->updateList();
}
void PvarMenu::on_actionDelete_triggered() {
    QList<QListWidgetItem *> items = ui->listWidget->selectedItems();
    for(auto item: items) {
        QString name = item->data(Qt::DisplayRole).toString().split("\t")[0];
        this->proto->pvars->erase(name.toStdString());
        int row = ui->listWidget->row(item);
        ui->listWidget->takeItem(row);
        delete item;
    }
}
void PvarMenu::on_actionShow_Cells_triggered() {
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
