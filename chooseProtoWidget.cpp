#include <QSlider>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QRadioButton>
#include <QGroupBox>

#include "chooseProtoWidget.h"
#include "currentClampProtocol.h"
#include "voltageClampProtocol.h"

chooseProtoWidget::chooseProtoWidget(QWidget* parent) {
    this->parent = parent;
    this->Initialize();
}

void chooseProtoWidget::Initialize() {
    this->proto = new voltageClamp();
    QStringList cell_options;
    cell_type = new QComboBox();
    QHBoxLayout* cellLayout = new QHBoxLayout();
    QLabel* simvars_label = new QLabel("Cell Type");
    auto cell_opitons_stl = proto->cellOptions();
    for(auto im = cell_opitons_stl.begin(); im != cell_opitons_stl.end(); im++) {
        cell_options << im->c_str();
    }
    cell_type->addItems(cell_options);
    cellLayout->addWidget(simvars_label);
    cellLayout->addWidget(cell_type);

    clampType = new QButtonGroup();
    QGroupBox* clampTypeBox = new QGroupBox("Clamp Type");
    auto startingButton = new QRadioButton("Voltage Clamp");
    startingButton->setChecked(true);
    clampType->addButton(startingButton, 0);
    clampType->addButton(new QRadioButton("Current Clamp"), 1);
    QVBoxLayout* clampLayout = new QVBoxLayout();
    {    
    auto list = clampType->buttons();
    for(auto it = list.begin(); it != list.end(); it++) {
        clampLayout->addWidget(*it);
    }
    }
    clampTypeBox->setLayout(clampLayout);

    QVBoxLayout* centralLayout = new QVBoxLayout();
    centralLayout->addLayout(cellLayout);
    centralLayout->addWidget(clampTypeBox);
    this->setLayout(centralLayout);
    connect(clampType, SIGNAL(buttonPressed(int)), this, SLOT(changeProto(int)));
    connect(cell_type, SIGNAL(currentIndexChanged(QString)), this, SLOT(changeCell(QString)));
}
Protocol* chooseProtoWidget::getCurrentProto() {
    return proto;
}
void chooseProtoWidget::setCurrentProto(Protocol* proto) {
    this->proto = proto;
}
void chooseProtoWidget::changeProto(int value) {
    Cell* old_cell = this->proto->cell->clone();
    string datadir = this->proto->datadir;
    switch(value) {
    case 0:
        this->proto = new voltageClamp();
    break;
    case 1:
        this->proto = new CurrentClamp();
    break;
    }

    this->proto->cell = old_cell;
    this->proto->datadir = datadir;
    emit protocolChanged(this->proto);
}

void chooseProtoWidget::changeCell(QString name) {
    this->proto->pars["celltype"].set(name.toStdString());
    emit cell_type_changed();
}

void chooseProtoWidget::cellChangedSlot() {
    int index = cell_type->findText(this->proto->pars["celltype"].get().c_str());
    if(index != -1) {
        cell_type->setCurrentIndex(index);
    }
}
