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
    explicit GridSetupWidget(GridProtocol* initial_proto, QWidget* parent = 0);
    ~GridSetupWidget() {}
    void setGrid(Grid* grid);
    Grid* getGrid();
	GridProtocol* getProtocol();
	QTableView* view();
	GridModel* getModel();
  signals:
    void cellChanged(Cell*);
  private:
    void createMenu();

	Ui::GridSetupWidget *ui;
    
    GridProtocol* proto;
    QWidget* parent;
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
