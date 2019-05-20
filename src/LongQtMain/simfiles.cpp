#include "simfiles.h"
#include <list>
#include <string>

SimFiles::SimFiles(std::shared_ptr<LongQt::Protocol> initial_proto,
                   QWidget *parent)
    : SimvarMenu(initial_proto, parent) {
  this->setNames({{"cellStateFile", "simvarfile"},
                  {"cellStateDir", "datadir"},
                  {"readcellstate", "writecellstate"}});
}
