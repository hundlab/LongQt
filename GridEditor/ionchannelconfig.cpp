#include <QStringList>

#include "ionchannelconfig.h"
#include "ui_ionchannelconfig.h"

IonChannelConfig::IonChannelConfig(QTableView* view, gridProtocol* proto, QWidget *parent) :
	QWidget(parent),
	ui(new Ui::IonChannelConfig)
{
	ui->setupUi(this);
	this->view = view;
	this->model = (GridModel*)view->model();
	this->proto = proto;
	this->cellChanged();
	connect(this->model, &GridModel::cell_type_changed, this, &IonChannelConfig::cellChanged);
}

IonChannelConfig::~IonChannelConfig()
{
	delete ui;
}

void IonChannelConfig::updateList() {
	QStringList toAdd;
	this->ui->listWidget->clear();
	for(auto& pvar : this->proto->new_pvars) {
		toAdd += pvar.first.c_str();
	}
	this->ui->listWidget->addItems(toAdd);
}

void IonChannelConfig::cellChanged() {
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
	ui->maxDist->setEnabled(state);
	ui->maxDistLabel->setEnabled(state);
	ui->maxVal->setEnabled(state);
	ui->maxValLabel->setEnabled(state);
}

void IonChannelConfig::on_addButton_clicked()
{
	string type = ui->ionChannelType->currentText().toStdString();
	gridProtocol::MIonChanParam toAdd;
	if(ui->randomize->checkState() == 0) {
		toAdd.dist = gridProtocol::Distribution::none;
		toAdd.val[0] = ui->startVal->value();
		toAdd.val[1] = ui->incAmt->value();
	} else {
		toAdd.val[0] = ui->mean->value();
		toAdd.val[1] = ui->stdDev->value();
		if(ui->normalDist->isChecked()) {
			toAdd.dist = gridProtocol::Distribution::normal;
		} else {
			toAdd.dist = gridProtocol::Distribution::lognormal;
		}
	}
	this->setIonChannels(ui->maxDist->value(), ui->maxVal->value(), toAdd);
	this->proto->new_pvars[type] = toAdd;
	this->updateList();
}
void IonChannelConfig::setIonChannels(int maxDist, double maxVal, gridProtocol::MIonChanParam ionConf) {
	this->getInitial(); 
	int i = 0;
	double val = 0;
	while(current.size() > 0 && i <= maxDist) {
		for(const pair<int,int>& e: this->current) {
			switch(ionConf.dist) {
				case gridProtocol::Distribution::none:
					val = ionConf.val[0]+ionConf.val[1]*i;
					if(val > maxVal) {
						val = maxVal;
					}
					break;
				case gridProtocol::Distribution::normal: {
					normal_distribution<double> distribution(ionConf.val[0],ionConf.val[1]);
					val = distribution(proto->generator);
					break;
														 }
				case gridProtocol::Distribution::lognormal: {
					lognormal_distribution<double> logdistribution(ionConf.val[0], ionConf.val[1]);
					val = logdistribution(proto->generator);
					break;
															}
			}
			ionConf.cells[make_pair(e.first,e.second)] = val;
			this->visited.insert(e);
		}
		++i;
		this->getNext();	
	}
	this->current.clear();
	this->visited.clear();
}

void IonChannelConfig::getInitial() {
	auto selected = this->view->selectionModel()->selectedIndexes();
	for(auto toAdd : selected) {
		this->add({toAdd.column(),toAdd.row()},this->current);
	}
}

void IonChannelConfig::getNext() {
	set<pair<int,int>> next;
	for(const pair<int,int>& e: this->current) {
		this->add({e.first,e.second+1}, next);
		this->add({e.first,e.second-1}, next);
		this->add({e.first+1,e.second}, next);
		this->add({e.first-1,e.second}, next);
	}
	this->current = next;
}
//need to check for duplicates
void IonChannelConfig::add(pair<int,int> e, set<pair<int,int>>& next) {
	//check for out of bounds
	if(e.second<0||e.first<0||e.second>=this->model->rowCount()||e.first>=this->model->columnCount()) {
		return;
	}
	if(this->visited.count(e) == 0) {
		next.insert(e);
	}

}

