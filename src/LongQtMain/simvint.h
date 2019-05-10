#ifndef SIMVINT_H
#define SIMVINT_H

#include "simvar.h"

#include <QSpinBox>
namespace LQ = LongQt;

class SimvInt : public Simvar {
 public:
  explicit SimvInt(std::shared_ptr<LQ::Protocol> proto, std::string name,
                   QString units, QWidget* parent = 0);
  ~SimvInt();

 private:
  virtual void update_ui();
  void update_model(int value);

  QSpinBox* widg;
};

#endif  // SIMVINT_H
