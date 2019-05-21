#ifndef STIMULATIONSETTINGS_H
#define STIMULATIONSETTINGS_H

#include <QWidget>
#include <memory>
#include "protocol.h"
#include "simvarmenu.h"

class StimulationSettings : public SimvarMenu {
 public:
  StimulationSettings(std::shared_ptr<LQ::Protocol> initial_proto,
                      QWidget* parent = nullptr);
};

#endif  // STIMULATIONSETTINGS_H
