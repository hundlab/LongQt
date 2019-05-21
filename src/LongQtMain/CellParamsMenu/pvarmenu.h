/*
 * The single cell equivelant of ion channel channel config
 * allows for cell->pars ending in Factor to be modified
 */
#ifndef PVARMENU_H
#define PVARMENU_H

#include <QMap>
#include <QPointer>
#include <QString>
#include <QWidget>
#include "addsinglecellpvar.h"
#include "grid.h"
#include "protocol.h"
#include "simvint.h"
namespace LQ = LongQt;

namespace Ui {
class PvarMenu;
}

class PvarMenu : public QWidget {
  Q_OBJECT

 public:
  explicit PvarMenu(std::shared_ptr<LQ::Protocol> proto, QWidget *parent = 0);
  virtual ~PvarMenu();

 protected:
  void updateList();

  std::shared_ptr<LQ::Protocol> proto = 0;
  Ui::PvarMenu *ui;

 private:
  QPointer<AddSingleCellPvar> addmenu;
  QMap<QString, QMap<QString, QString>> pvarsDescriptions;
  QPointer<SimvInt> numTrials;

  void hideNumTrials(bool hide);

 public slots:
  void changeProto(std::shared_ptr<LQ::Protocol> proto);
  void changeCell(std::shared_ptr<LQ::Cell> cell);

 private slots:
  void on_actionDelete_triggered();
  void on_actionShow_Cells_triggered();

  void on_addButton_triggered();
  void on_removeButton_triggered();
  void on_infoButton_triggered();
  void on_refreshButton_triggered();
  void on_treeWidget_itemSelectionChanged();
};
#endif
