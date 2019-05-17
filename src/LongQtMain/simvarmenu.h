#ifndef SIMVARMENU_H
#define SIMVARMENU_H

#include "voltageclampsetupwidget.h"

#include <simvar.h>

#include <QWidget>

#include <gridsetupwidget.h>
#include <protocol.h>
#include <QFormLayout>
namespace LQ = LongQt;

namespace Ui {
class SimvarMenu;
}

class SimvarMenu : public QWidget {
  Q_OBJECT

 public:
  explicit SimvarMenu(std::shared_ptr<LQ::Protocol> initial_proto,
                      QWidget* parent = 0);
  void createMenu();
  ~SimvarMenu();

  void removeGrid();

 private:
  typedef std::function<std::pair<QWidget*, Simvar*>(
      std::shared_ptr<LQ::Protocol>, std::string)>
      SimvarInitializer;

  Ui::SimvarMenu* ui;
  std::shared_ptr<LQ::Protocol> proto;
  // Buttons & their labels
  QMap<QString, QMap<QString, QString>> descriptions;
  QMap<QString, Simvar*> simvars;

  //  GridSetupWidget* grid = 0;
  //  VoltageClampSetupWidget* voltageClamp = 0;
  // initalizer
  void initialize(const std::map<std::string, LQ::GetSetRef>::iterator it);
  // screen functions
  void update_menu();  // make menu match pars
  bool signalCellTypeChange = true;

 public slots:
  void changeProto(std::shared_ptr<LQ::Protocol> proto);
  void changeCell(std::shared_ptr<LQ::Cell>);
  void reset();
  void setWorkingDir(QDir& dir);
 signals:
  void cellChanged(std::shared_ptr<LQ::Cell>);
  void updated();
};

#endif  // SIMVARMENU_H
