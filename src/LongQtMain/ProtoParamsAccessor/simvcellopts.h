#ifndef SIMVCELLOPTS_H
#define SIMVCELLOPTS_H

#include "simvar.h"

#include <QCheckBox>
#include <QGroupBox>
#include <QMap>
#include <QVBoxLayout>
namespace LQ = LongQt;

class SimvCellOpts : public Simvar {
 public:
  explicit SimvCellOpts(std::shared_ptr<LQ::Protocol> proto, std::string name,
                        QGroupBox* parent);

  ~SimvCellOpts();

  void update_ui() override;
  void update_model(std::string name, bool value);
  void changeCell(std::shared_ptr<LQ::Cell>) override;

 private:
  void setup(QGroupBox* parent);

  QVBoxLayout* vbox;
  QMap<std::string, QCheckBox*> checkMap;
};

#endif  // SimvCellOpts_H
