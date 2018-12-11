#include "simvar.h"
#include "oneitemlayout.h"
using namespace std;
using namespace LongQt;

Simvar::Simvar(shared_ptr<Protocol> proto, string name, QWidget *parent)
    : QWidget(parent) {
  this->name = name;
  this->proto = proto;
}

void Simvar::changeProto(shared_ptr<Protocol> proto) { this->proto = proto; }

void Simvar::changeCell(shared_ptr<Cell>) { this->update_ui(); }

Simvar::~Simvar() {}
