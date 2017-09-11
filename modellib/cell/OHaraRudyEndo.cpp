#include "OHaraRudyEndo.h"

OHaraRudyEndo::OHaraRudyEndo() {
	this->Initialize();
};

OHaraRudyEndo::~OHaraRudyEndo() {};

OHaraRudyEndo::OHaraRudyEndo(OHaraRudyEndo& toCopy) : OHaraRudy(toCopy) {
    this->Initialize(); 
    CellKernel::copyVarPar(toCopy);
}

void OHaraRudyEndo::Initialize() {
	celltype = OHaraRudy::endo;
	this->makemap();
}

OHaraRudyEndo *OHaraRudyEndo::clone() {
    return  new OHaraRudyEndo(*this);
}

const char *OHaraRudyEndo::type() const
{
    return "Human Ventricular Endo (O'Hara-Rudy 2011)";
}
