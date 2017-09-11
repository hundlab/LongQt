/*
 * widget for modifying the grid
 */
#ifndef gridSetupWidget_H
#define gridSetupWidget_H

#include <QWidget>
#include <QTableView>
#include <QPushButton>
#include <QComboBox>
#include <QCheckBox>
#include <QSpinBox>

#include "gridProtocol.h"
#include "node.h"
#include "grid.h"
#include "gridCell.h"
#include "gridModel.h"

using namespace std;

namespace Ui {
class GridSetupWidget;
}

class GridSetupWidget : public QWidget {
Q_OBJECT
  public:
    explicit GridSetupWidget(QWidget* parent = 0);
    explicit GridSetupWidget(shared_ptr<GridProtocol> initial_proto, QWidget* parent = 0);
    ~GridSetupWidget();
    void setGrid(Grid* grid);
    Grid* getGrid();
	shared_ptr<GridProtocol> getProtocol();
	QTableView* view();
	GridModel* getModel();
  signals:
    void cellChanged(shared_ptr<Cell>);
  private:
    void createMenu();

	Ui::GridSetupWidget *ui;

    shared_ptr<GridProtocol> proto;
    Grid* grid;

	GridModel* model;
  private slots:
    void on_chooseType_currentIndexChanged(QString type);
    void on_toggleMeasure_clicked();
    void on_toggleStim_clicked();
  public slots:
	void reset();
};
#endif
