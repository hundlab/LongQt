#include <QSlider>
#include <QHBoxLayout>

#include "chooseProtoWidget.h"
#include "currentClampProtocol.h"
#include "voltageClampProtocol.h"

chooseProtoWidget::chooseProtoWidget(QWidget* parent) {
    this->Initialize(parent);
}

void chooseProtoWidget::Initialize(QWidget* parent) {
    this->parent = parent;
    this->proto = new voltageClamp();
    QStringList cell_options;
    cell_type = new QComboBox();
    QLabel* simvars_label = new QLabel("celltype");
    auto cell_opitons_stl = proto->cellOptions();
    for(auto im = cell_opitons_stl.begin(); im != cell_opitons_stl.end(); im++) {
        cell_options << im->c_str();
    }
    cell_type->addItems(cell_options);

    clampType = new QSlider();
    clampType->setMaximum(1);
    clampType->setMinimum(0);
    clampType->setOrientation(Qt::Horizontal);
    voltage = new QLabel("Voltage Clamp");
    current = new QLabel("Current Clamp");
    QHBoxLayout* centralLayout = new QHBoxLayout();
    centralLayout->addWidget(simvars_label);
    centralLayout->addWidget(cell_type);
    centralLayout->addWidget(voltage);
    centralLayout->addWidget(clampType);
    centralLayout->addWidget(current);
    this->setLayout(centralLayout);
    connect(clampType, SIGNAL(valueChanged(int)), this, SLOT(changeProto(int)));
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
