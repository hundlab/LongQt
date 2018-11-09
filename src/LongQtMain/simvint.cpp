#include "oneitemlayout.h"
#include "simvint.h"

#include <protocol.h>
#include <QGridLayout>
using namespace std;
using namespace LongQt;

SimvInt::SimvInt(shared_ptr<Protocol> proto, string name, QWidget *parent) :
    Simvar(proto,name,parent)
{
    this->widg = new QSpinBox();
    auto layout = new OneItemLayout(this);
    layout->addWidget(widg);
    widg->setRange(/*std::numeric_limits<double>::min()*/ -100000,std::numeric_limits<int>::max());

    connect(widg, QOverload<int>::of(&QSpinBox::valueChanged), this, &SimvInt::update_model);
}

void SimvInt::update_ui() {
    string value = proto->parsStr(this->name);
    widg->setValue(std::stoi(value));
    emit updated();
}

void SimvInt::update_model(int value) {
    proto->parsStr(name,to_string(value));
}

SimvInt::~SimvInt()
{}
