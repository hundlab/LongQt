#include <QStringList>
#include <QMessageBox>

#include "ionchannelconfig.h"
#include "ui_ionchannelconfig.h"

IonChannelConfig::IonChannelConfig(QTableView* view, GridProtocol* proto, QWidget *parent) :
	QWidget(parent),
	ui(new Ui::IonChannelConfig)
{
	ui->setupUi(this);
	this->view = view;
	this->model = (GridModel*)view->model();
	this->proto = proto;
	this->cellChanged();
	this->updateList();
//	ui->maxDist->setValue(0);
//	ui->maxVal->setValue(1000);
    ui->listWidget->addAction(ui->actionDelete);
    ui->listWidget->addAction(ui->actionShow_Cells);
	connect(this->model, &GridModel::cell_type_changed, this, &IonChannelConfig::cellChanged);
}

IonChannelConfig::~IonChannelConfig()
{
	delete ui;
}

void IonChannelConfig::updateList() {
	QStringList toAdd;
	string info;
	this->ui->listWidget->clear();
	for(auto& pvar : this->proto->new_pvars) {
		switch(pvar.second.dist) {
			case GridProtocol::Distribution::none:
				info = (pvar.first + "\tIncrementing\tInitial Value: "+to_string(pvar.second.val[0])+
						"\tIncrement Amount: "+to_string(pvar.second.val[1]));
				break;
			case GridProtocol::Distribution::normal:
				info = (pvar.first + "\tNormal Distribution\t Mean: "+to_string(pvar.second.val[0])+
						"\tStandard Deviation: "+to_string(pvar.second.val[1]));
				break;
			case GridProtocol::Distribution::lognormal:
				info = (pvar.first + "\tLognormal Distribution\t Mean: "+to_string(pvar.second.val[0])+
						"\tStandard Deviation: "+to_string(pvar.second.val[1]));
				break;
		}
		toAdd += info.c_str();
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
	GridProtocol::MIonChanParam toAdd;
	if(ui->randomize->checkState() == 0) {
		toAdd.dist = GridProtocol::Distribution::none;
		toAdd.val[0] = ui->startVal->value();
		toAdd.val[1] = ui->incAmt->value();
	} else {
		toAdd.val[0] = ui->mean->value();
		toAdd.val[1] = ui->stdDev->value();
		if(ui->normalDist->isChecked()) {
			toAdd.dist = GridProtocol::Distribution::normal;
		} else {
			toAdd.dist = GridProtocol::Distribution::lognormal;
		}
	}
	if(ui->multiple->isChecked()) {
		maxDist = ui->maxDist->value();
		maxVal = ui->maxVal->value();
	}
	this->setIonChannels(maxDist, maxVal, toAdd);
	if(toAdd.cells.size() > 0) {
		this->proto->new_pvars[type] = toAdd;
	}
	this->updateList();
}
void IonChannelConfig::setIonChannels(int maxDist, double maxVal, GridProtocol::MIonChanParam& ionConf) {
	this->getInitial(); 
	int i = 0;
	double val = 0;
	while(current.size() > 0 && i <= maxDist) {
		for(const pair<int,int>& e: this->current) {
			switch(ionConf.dist) {
				case GridProtocol::Distribution::none:
					val = ionConf.val[0]+ionConf.val[1]*i;
				break;
				case GridProtocol::Distribution::normal: 
					{
						normal_distribution<double> distribution(ionConf.val[0],ionConf.val[1]);
						val = distribution(proto->generator);
						break;
					}
				case GridProtocol::Distribution::lognormal:
					{
						lognormal_distribution<double> logdistribution(ionConf.val[0], ionConf.val[1]);
						val = logdistribution(proto->generator);
						break;
					}
			}
			if(val > maxVal) {
				val = maxVal;
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

void IonChannelConfig::on_actionDelete_triggered() {
    QList<QListWidgetItem *> items = ui->listWidget->selectedItems();
    for(auto item: items) {
        QString name = item->data(Qt::DisplayRole).toString().split("\t")[0];
        this->proto->new_pvars.erase(name.toStdString());
        int row = ui->listWidget->row(item);
        ui->listWidget->takeItem(row);
        delete item;
    }
}
void IonChannelConfig::on_actionShow_Cells_triggered() {
    QList<QListWidgetItem *> items = ui->listWidget->selectedItems();
    QString text = "(X <column>, Y <row>)\n";
    for(auto item: items) {
        QString name = item->data(Qt::DisplayRole).toString().split("\t")[0];
        text += name+"\n";
        GridProtocol::MIonChanParam param = this->proto->new_pvars[name.toStdString()];
        for(auto& cell: param.cells) {
            text += "(" + QString::number(cell.first.first) +","+
                    QString::number(cell.first.second) +") = "+QString::number(cell.second)+"\n";
        }
    }
    QMessageBox msgBox;
    msgBox.setText(text);
    msgBox.exec();
}
