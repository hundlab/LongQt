#include "OHaraRudyM.h"

OHaraRudyM::OHaraRudyM() {
	this->Initialize();
};

OHaraRudyM::~OHaraRudyM() {};

OHaraRudyM::OHaraRudyM(OHaraRudyM& toCopy) : OHaraRudy(toCopy) {
    this->Initialize(); 
    CellKernel::copyVarPar(toCopy);
}

void OHaraRudyM::Initialize() {
	celltype = OHaraRudy::M;
	this->makemap();
}

OHaraRudyM* OHaraRudyM::clone() {
    return new OHaraRudyM(*this);
}

const char *OHaraRudyM::type() const
{
    return "Human Ventricular (Ten Tusscher 2004)";
}
