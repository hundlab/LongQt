#include "oneitemlayout.h"
#include "simvdouble.h"

#include <protocol.h>

SimvDouble::SimvDouble(shared_ptr<Protocol> proto, string name, QWidget *parent) :
    Simvar(proto,name,parent)
{
    this->widg = new QDoubleSpinBox(this);
    auto layout = new OneItemLayout(this);
    layout->addWidget(widg);
    widg->setRange(/*std::numeric_limits<double>::min()*/ -100000,std::numeric_limits<double>::max());

    connect(widg, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &SimvDouble::update_model);
}

void SimvDouble::update_ui() {
    string value = proto->pars[this->name].get();
    widg->setValue(std::stod(value));
    emit updated();
}

void SimvDouble::update_model(double value) {
    proto->pars.at(name).set(to_string(value));
}

SimvDouble::~SimvDouble()
{}
