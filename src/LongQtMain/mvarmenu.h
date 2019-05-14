/*
 * Measurement variables are the cell->vars which will have basic measurements
 * taken of them durring the simulation
 * This widget alows for them to be added and removed from the mvars list
 */
#ifndef MVARMENU_H
#define MVARMENU_H

#include <QMap>
#include <QTreeWidgetItem>
#include <QVector>
#include <QWidget>

#include "protocol.h"
#include "dataoutputselectionmodel.h"
namespace LQ = LongQt;

namespace Ui {
class MvarMenu;
}

class MvarMenu : public QWidget {
  Q_OBJECT
 public:
  explicit MvarMenu(std::shared_ptr<LQ::Protocol> proto, QWidget* parent = 0);
  ~MvarMenu();

 private:
  void setupMenu();

  std::shared_ptr<LQ::Protocol> proto;
  Ui::MvarMenu* ui;
  DataOutputSelectionModel* model;

 public slots:
  void changeProto(std::shared_ptr<LQ::Protocol> proto);
  void changeCell(std::shared_ptr<LQ::Cell> cell);
  void reset();
};

#endif
