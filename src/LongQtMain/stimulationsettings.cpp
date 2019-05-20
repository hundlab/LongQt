#include "stimulationsettings.h"

StimulationSettings::StimulationSettings(
    std::shared_ptr<LQ::Protocol> initial_proto, QWidget *parent)
    : SimvarMenu(initial_proto, parent) {
    this->setNames({{"stimdur", "stimt", "stimval"}, {"bcl", "numstims", "paceflag"}});
}
