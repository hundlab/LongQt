#ifndef SIMVCELL_H
#define SIMVCELL_H

#include "simvar.h"

#include <QComboBox>
namespace LQ = LongQt;

class SimvCell : public Simvar {
 public:
  explicit SimvCell(std::shared_ptr<LQ::Protocol> proto, std::string name,
                    QWidget* parent = 0);
  ~SimvCell();

 private:
  virtual void update_ui();
  void update_model(QString value);

  QComboBox* widg;
};

#endif  // SimvCell_H
