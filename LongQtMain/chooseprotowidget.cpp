#include <QSlider>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QRadioButton>
#include <QGroupBox>

#include "ui_chooseprotowidget.h"
#include "chooseprotowidget.h"
#include "currentClampProtocol.h"
#include "voltageClampProtocol.h"
#include "gridProtocol.h"

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
    clampType->addButton(ui->currentClamp, 0);
    clampType->addButton(ui->voltageClamp, 1);
    clampType->addButton(ui->grid, 2);

    connect(clampType, SIGNAL(buttonPressed(int)), this, SLOT(changeProto(int)));
}
Protocol* ChooseProtoWidget::getCurrentProto() {
    return proto;
}
void ChooseProtoWidget::resetProto() {
    this->changeProto(0);
}
void ChooseProtoWidget::changeProto(int value) {
    Cell* old_cell = this->proto->cell->clone();
    string datadir = this->proto->datadir;
    string cellStateDir = this->proto->cellStateDir;
    if(old_cell->type == string("gridCell")) {
        ui->cellType->setEnabled(true);
        ui->cellType->removeItem(ui->cellType->count() -1);
    }
    switch(value) {
    case 0:
        this->proto = new CurrentClamp();
        this->proto->cell = old_cell;
    break;
    case 1:
        this->proto = new VoltageClamp();
        this->proto->cell = old_cell;
    break;
    case 2:
        this->proto = new GridProtocol();
        ui->cellType->addItem("gridCell");
        ui->cellType->setEnabled(false);
        emit cell_type_changed();
    break;
    }

    if(old_cell->type == string("gridCell")) {
        on_cellType_currentIndexChanged(defaultCell);
    }
    this->proto->datadir = datadir;
    this->proto->cellStateDir = cellStateDir;
    emit protocolChanged(this->proto);
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
