#ifndef GRIDWRAPPER_H
#define GRIDWRAPPER_H

#include <QPointer>
#include <QWidget>
#include <memory>
#include "gridsetupwidget.h"

namespace LQ = LongQt;

class GridWrapper : public QWidget {
  Q_OBJECT
 public:
  explicit GridWrapper(std::shared_ptr<LQ::Protocol> initial_proto,
                       QWidget *parent = nullptr);

  void changeProto(std::shared_ptr<LQ::Protocol>);

 private:
  QPointer<GridSetupWidget> grid;

 signals:
  void cellChanged(std::shared_ptr<LQ::Cell>);
  void widgetSet(bool);
};

#endif  // GRIDWRAPPER_H
