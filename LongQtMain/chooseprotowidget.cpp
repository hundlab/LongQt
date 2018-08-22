#include <QSlider>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QRadioButton>
#include <QGroupBox>
#include <QFileDialog>

#include "ui_chooseprotowidget.h"
#include "chooseprotowidget.h"
#include "cellutils.h"
#include "currentClampProtocol.h"
#include "settingsIO.h"

ChooseProtoWidget::ChooseProtoWidget(QWidget* parent) : QWidget(parent),
	ui(new Ui::ChooseProtoWidget)
{
	ui->setupUi(this);
    this->parent = parent;
    this->Initialize();
}

void ChooseProtoWidget::Initialize() {
    this->proto.reset(new CurrentClamp());
    this->defaultCell = this->proto->cell()->type();

    for(auto& opt: proto->cellOptions()) {
        ui->cellType->addItem(opt.c_str());
    }
    this->changeCell(proto->cell());

    clampType = new QButtonGroup();
	int i = 3;
	for(auto& protocol: CellUtils::protoMap) {
		if(protocol.first == string("Current Clamp Protocol")) {
			clampType->addButton(ui->currentClamp,0);
			this->protoNumMap.insert(0,protocol.first);
		} else if(protocol.first == string("Voltage Clamp Protocol")) {
			clampType->addButton(ui->voltageClamp,1);
			this->protoNumMap.insert(1,protocol.first);
		} else if(protocol.first == string("Grid Protocol")) {
			clampType->addButton(ui->grid,2);
			this->protoNumMap.insert(2,protocol.first);
		} else {
			QRadioButton* protoChoice = new QRadioButton(protocol.first.c_str());
			clampType->addButton(protoChoice,i);
			ui->groupBoxLayout->addWidget(protoChoice);
			this->protoNumMap.insert(i,protocol.first);
			i++;
		}
	}

    connect(clampType, SIGNAL(buttonPressed(int)), this, SLOT(changeProto(int)));
}
shared_ptr<Protocol> ChooseProtoWidget::getCurrentProto() {
    return proto;
}
void ChooseProtoWidget::resetProto() {
    this->changeProto(0);
}
void ChooseProtoWidget::changeProto(int value) {
	this->changeProto(
        CellUtils::protoMap.at(this->protoNumMap[value])()
        ,true);
}
void ChooseProtoWidget::changeProto(string name) {
	this->changeProto(shared_ptr<Protocol>(CellUtils::protoMap.at(name)()),
        true);
}
void ChooseProtoWidget::changeProto(shared_ptr<Protocol> newProto, bool raise) {
    if(newProto == this->proto)
        return;
    newProto->cellStateDir = this->proto->cellStateDir;
    newProto->datadir = this->proto->datadir;
    string old_cell = this->proto->cell()->type();
    newProto->cell(old_cell);

	this->proto = newProto;

	if(raise) {
    	emit protocolChanged(this->proto);
        emit cellChanged(proto->cell());
	}

	this->updateMenu();
}
void ChooseProtoWidget::updateMenu() {
    bool oldState = ui->cellType->blockSignals(true);
	int typeNum = this->protoNumMap.keys(this->proto->type()).at(0);
	auto button = this->clampType->button(typeNum);
	if(button)
		button->setChecked(true);
    ui->cellType->clear();
    for(auto& opt: proto->cellOptions()) {
        ui->cellType->addItem(opt.c_str());
    }
    ui->cellType->setCurrentText(this->proto->cell()->type());
    ui->cellType->blockSignals(oldState);
}

void ChooseProtoWidget::on_cellType_currentIndexChanged(QString name) {
    this->proto->pars["celltype"].set(name.toStdString());
    CellUtils::set_default_vals(*this->proto);
    emit cellChanged(proto->cell());
}

void ChooseProtoWidget::changeCell(shared_ptr<Cell> cell) {
    if(cell != proto->cell()) {
		qWarning("ChooseProtoWidget: Protocol cell does not match new cell");
	}
    int index = ui->cellType->findText(this->proto->pars["celltype"].get().c_str());
    if(index != -1 && ui->cellType->currentIndex() != index) {
        bool state = ui->cellType->blockSignals(true);
        ui->cellType->setCurrentIndex(index);
        ui->cellType->blockSignals(state);
    }
}

void ChooseProtoWidget::on_readSettings_clicked() {
    QString fileName = QFileDialog::getOpenFileName(this,"Choose Settings file",proto->datadir.absolutePath());
    if (!fileName.isEmpty()){ 
		SettingsIO* settingsMgr = SettingsIO::getInstance();
        settingsMgr->readSettings(fileName,this->proto);
		this ->proto = settingsMgr->lastProto;
		emit protocolChanged(this->proto);
        emit cellChanged(proto->cell());
		this->updateMenu();
	}
}
