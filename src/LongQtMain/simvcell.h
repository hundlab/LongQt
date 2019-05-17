#ifndef SIMVCELL_H
#define SIMVCELL_H

#include <QObject>
#include "simvar.h"

#include <QComboBox>
namespace LQ = LongQt;

class SimvCell : public Simvar {
  Q_OBJECT
 public:
  explicit SimvCell(std::shared_ptr<LQ::Protocol> proto, std::string name,
                    QComboBox* parent);
  ~SimvCell();
  void update_ui() override;
  void update_model(QString value);
  void changeProto(std::shared_ptr<LongQt::Protocol> proto) override;

 signals:
  void cellChanged(std::shared_ptr<LQ::Cell>);
};

#endif  // SimvCell_H
