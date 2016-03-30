#include <QSlider>
#include <QHBoxLayout>

#include "chooseProtoWidget.h"
#include "CurrentClampProtocol.h"
#include "voltageClampProtocol.h"

chooseProtoWidget::chooseProtoWidget(QWidget* parent) {
    this->parent = parent;
    this->proto = new Protocol();
    clampType = new QSlider();
    clampType->setMaximum(1);
    clampType->setMinimum(0);
    clampType->setOrientation(Qt::Horizontal);
    voltage = new QLabel("Voltage Clamp");
    current = new QLabel("Current Clamp");
    QHBoxLayout* centralLayout = new QHBoxLayout();
    centralLayout->addWidget(voltage);
    centralLayout->addWidget(clampType);
    centralLayout->addWidget(current);
    this->setLayout(centralLayout);
    connect(clampType, SIGNAL(valueChanged(int)), this, SLOT(changeProto(int)));
}
Protocol* chooseProtoWidget::getCurrentProto() {
    return proto;
}
void chooseProtoWidget::setCurrentProto(Protocol* proto) {
    this->proto = proto;
}
void chooseProtoWidget::changeProto(int value) {
    switch(value) {
    case 0:
        this->proto = new VoltageClamp();
    break;
    case 1:
        this->proto = new CurrentClamp();
    break;
    }
    emit protocolChanged();
}
