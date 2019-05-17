#ifndef SIMVBOOL_H
#define SIMVBOOL_H

#include <QCheckBox>
#include "simvar.h"

namespace LQ = LongQt;

class SimvBool : public Simvar {
 public:
  explicit SimvBool(std::shared_ptr<LQ::Protocol> proto, std::string name,
                    QCheckBox* parent);

  ~SimvBool();
  void update_ui() override;
  void update_model(int value);
};

#endif  // SimvBool_H
