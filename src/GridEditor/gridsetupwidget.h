/*
 * widget for modifying the grid
 */
#ifndef gridSetupWidget_H
#define gridSetupWidget_H

#include <QCheckBox>
#include <QComboBox>
#include <QPushButton>
#include <QSpinBox>
#include <QTableView>
#include <QWidget>

#include "grid.h"
#include "gridCell.h"
#include "gridModel.h"
#include "gridProtocol.h"
#include "node.h"
namespace LQ = LongQt;

namespace Ui {
class GridSetupWidget;
}

class GridSetupWidget : public QWidget {
  Q_OBJECT
 public:
  explicit GridSetupWidget(QWidget* parent = 0);
  explicit GridSetupWidget(std::shared_ptr<LQ::GridProtocol> initial_proto,
                           QWidget* parent = 0);
  ~GridSetupWidget();
  void setGrid(LQ::Grid* grid);
  LQ::Grid* getGrid();
  std::shared_ptr<LQ::GridProtocol> getProtocol();
  QTableView* view();
  GridModel* getModel();
 signals:
  void cellChanged(std::shared_ptr<LQ::Cell>);

 private:
  void createMenu();

  Ui::GridSetupWidget* ui;

  std::shared_ptr<LQ::GridProtocol> proto;
  LQ::Grid* grid;

  GridModel* model;
 private slots:
  void on_chooseType_currentIndexChanged(QString type);
  void on_toggleMeasure_clicked();
  void on_toggleStim_clicked();
 public slots:
  void reset();
};
#endif
