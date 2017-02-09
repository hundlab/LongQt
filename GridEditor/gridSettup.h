/*
 * widget for modifying the grid
 */
#ifndef gridSetupWidget_H
#define gridSetupWidget_H

#include <QWidget>
#include <QDir>
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

class GridSetupWidget : public QWidget {
Q_OBJECT
  public:
    GridSetupWidget(QWidget* parent = 0);
    GridSetupWidget(GridProtocol* initial_proto, QDir workingDir, QWidget* parent = 0);
    ~GridSetupWidget() {}
    void setGrid(Grid* grid);
    Grid* getGrid();
	GridProtocol* getProtocol();
	QTableView* view();
	GridModel* getModel();
  signals:
    void cell_type_changed();
  private:
    void createMenu();
    
    GridProtocol* proto;
    QDir workingDir;
    QWidget* parent;
    Grid* grid;

    QTableView* cellGrid;
	GridModel* model;
    QSpinBox* columnInt;
    QPushButton* addColumnButton;
    QPushButton* removeColumnButton;
    QSpinBox* rowInt;
    QPushButton* addRowButton;
    QPushButton* removeRowButton;
    QPushButton* toggleMeasure;
    QPushButton* toggleStim;
    QComboBox* chooseType;
  private slots:
    void changeCellGroup(QString type);
    void toggleMeasurePressed();
    void toggleStimPressed();
  public slots:
	void reset();
};
#endif
