#ifndef SIMVARMENU_H
#define SIMVARMENU_H

#include "voltageclampsetupwidget.h"

#include <simvar.h>

#include <QPointer>
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
                      QWidget* parent = nullptr);
  virtual ~SimvarMenu();

  void setNames(std::list<std::list<std::string>> names);

  void update_menu();

 private:
  typedef std::function<std::pair<QWidget*, Simvar*>(
      std::shared_ptr<LQ::Protocol>, std::string)>
      SimvarInitializer;

  Ui::SimvarMenu* ui;
  std::shared_ptr<LQ::Protocol> proto;
  // Buttons & their labels
  QMap<QString, QPointer<Simvar>> simvars;
  std::list<std::list<std::string>> names;
  void createMenu();

 public:
  void changeProto(std::shared_ptr<LQ::Protocol> proto);
  void changeCell(std::shared_ptr<LQ::Cell>);
  void reset();
 signals:
  void cellChanged(std::shared_ptr<LQ::Cell>);
  void updated();
};

#endif  // SIMVARMENU_H
