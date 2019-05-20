#ifndef SIMFILES_H
#define SIMFILES_H

#include <QWidget>
#include <memory>
#include "protocol.h"
#include "simvarmenu.h"

namespace LQ = LongQt;

class SimFiles : public SimvarMenu {
 public:
  SimFiles(std::shared_ptr<LQ::Protocol> initial_proto,
           QWidget* parent = nullptr);
};

#endif  // SIMFILES_H
