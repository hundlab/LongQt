#ifndef SIMVINT_H
#define SIMVINT_H

#include "simvar.h"

#include <QSpinBox>
namespace LQ = LongQt;

class SimvInt : public Simvar {
 public:
  explicit SimvInt(std::shared_ptr<LQ::Protocol> proto, std::string name,
                   QSpinBox* parent = 0);
  ~SimvInt();

  void update_ui() override;
  void update_model(int value);
};

#endif  // SIMVINT_H
