#ifndef VOLTAGECLAMPSETUPWIDGET_H
#define VOLTAGECLAMPSETUPWIDGET_H

#include "voltageclampmodel.h"

#include <voltageClampProtocol.h>
#include <QWidget>
namespace LQ = LongQt;

namespace Ui {
class VoltageClampSetupWidget;
}

class VoltageClampSetupWidget : public QWidget {
  Q_OBJECT

 public:
  explicit VoltageClampSetupWidget(
      std::shared_ptr<LQ::VoltageClamp> initial_proto, QWidget *parent = 0);
  ~VoltageClampSetupWidget();

 private:
  void createMenu();

  VoltageClampModel *model;
  std::shared_ptr<LQ::VoltageClamp> proto;
  Ui::VoltageClampSetupWidget *ui;
};

#endif  // VOLTAGECLAMPSETUPWIDGET_H
