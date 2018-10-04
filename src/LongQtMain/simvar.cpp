#include "oneitemlayout.h"
#include "simvar.h"

Simvar::Simvar(shared_ptr<Protocol> proto, string name, QWidget *parent) :
    QWidget(parent)
{
    this->name = name;
    this->proto = proto;
}

void Simvar::changeProto(shared_ptr<Protocol> proto) {
    this->proto = proto;
}

void Simvar::changeCell(shared_ptr<Cell>) {
    this->update_ui();
}

Simvar::~Simvar()
{}
