#ifndef SIMVCELLOPTS_H
#define SIMVCELLOPTS_H

#include "simvar.h"

#include <QCheckBox>
#include <QGroupBox>
#include <QMap>
namespace LQ = LongQt;

class SimvCellOpts : public Simvar {
 public:
  explicit SimvCellOpts(std::shared_ptr<LQ::Protocol> proto, std::string name,
                        QWidget* parent = 0);
  ~SimvCellOpts();
  void createMenu();

 private:
  virtual void update_ui();
  void update_model(bool value);

  QMap<std::string, QCheckBox*> checkMap;
  QGroupBox* widg;
 public slots:
  virtual void changeCell(std::shared_ptr<LQ::Cell>);
};

#endif  // SimvCellOpts_H
