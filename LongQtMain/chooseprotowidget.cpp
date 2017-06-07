#include <QSlider>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QRadioButton>
#include <QGroupBox>

#include "ui_chooseprotowidget.h"
#include "chooseprotowidget.h"
#include "cellutils.h"
#include "currentClampProtocol.h"

ChooseProtoWidget::ChooseProtoWidget(QWidget* parent) : QWidget(parent),
	ui(new Ui::ChooseProtoWidget)
{
	ui->setupUi(this);
    this->parent = parent;
    this->Initialize();
}

void ChooseProtoWidget::Initialize() {
    this->proto = new CurrentClamp();
    this->defaultCell = this->proto->cell->type;

    QStringList cell_options;
    auto cell_opitons_stl = proto->cellOptions();
    for(auto im = cell_opitons_stl.begin(); im != cell_opitons_stl.end(); im++) {
        cell_options << im->c_str();
    }
    ui->cellType->addItems(cell_options);
    this->cellChangedSlot();

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
Protocol* ChooseProtoWidget::getCurrentProto() {
    return proto;
}
void ChooseProtoWidget::resetProto() {
    this->changeProto(0);
}
void ChooseProtoWidget::changeProto(int value) {
	this->changeProto(CellUtils::protoMap.at(this->protoNumMap[value])(),true);
}
void ChooseProtoWidget::changeProto(string name) {
	this->changeProto(CellUtils::protoMap.at(name)(),true);
}
void ChooseProtoWidget::changeProto(Protocol* newProto, bool raise) {
    string datadir = this->proto->datadir;
	Cell* old_cell = this->proto->cell->clone();
	string cellStateDir = this->proto->cellStateDir;
	if(old_cell->type == string("gridCell")) {
		ui->cellType->setEnabled(true);
		ui->cellType->removeItem(ui->cellType->count() -1);
	}
	this->proto = newProto;

	if(this->proto->cell->type == string("gridCell")) {
		ui->cellType->addItem("");
		ui->cellType->setEnabled(false);
		emit cell_type_changed();
	} else {
		this->proto->cell = old_cell;
	}
	if(old_cell->type == string("gridCell")) {
		on_cellType_currentIndexChanged(defaultCell);
	}
	this->proto->cellStateDir = cellStateDir;
	this->proto->datadir = datadir;
	if(raise) {
    	emit protocolChanged(this->proto);
	}
	int typeNum = this->protoNumMap.keys(this->proto->type).at(0);
	auto button = this->clampType->button(typeNum);
	if(button)
		button->setChecked(true);
}

void ChooseProtoWidget::on_cellType_currentIndexChanged(QString name) {
    this->proto->pars["celltype"].set(name.toStdString());
    emit cell_type_changed();
}

void ChooseProtoWidget::cellChangedSlot() {
    int index = ui->cellType->findText(this->proto->pars["celltype"].get().c_str());
    if(index != -1) {
        ui->cellType->setCurrentIndex(index);
    }
}
