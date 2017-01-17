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

class gridSetupWidget : public QWidget {
Q_OBJECT
  public:
	gridSetupWidget(QWidget* parent = 0);
    gridSetupWidget(gridProtocol* initial_proto, QDir workingDir, QWidget* parent = 0);
    ~gridSetupWidget() {}
    void setGrid(Grid* grid);
    Grid* getGrid();
	gridProtocol* getProtocol();
	QTableView* view();
	GridModel* getModel();
  signals:
    void cell_type_changed();
  private:
    void createMenu();
    
    gridProtocol* proto;
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
