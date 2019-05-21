#ifndef SIMVDOUBLE_H
#define SIMVDOUBLE_H

#include "simvar.h"

#include <QDoubleSpinBox>
namespace LQ = LongQt;

class SimvDouble : public Simvar {
 public:
  explicit SimvDouble(std::shared_ptr<LQ::Protocol> proto, std::string name,
                      QDoubleSpinBox* parent);

  ~SimvDouble();

  void update_ui() override;
  void update_model(double value);
};

#endif  // SIMVDOUBLE_H
