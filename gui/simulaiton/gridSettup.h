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

class gridNode : public QWidget {
Q_OBJECT
  public:
    gridNode(Node* node, int X, int Y, gridCell* parentCell);
    ~gridNode();
    Node* getNode();
    pair<int,int> getNodePair();
    void update(bool stim, bool meas);
    bool getStimStatus();
    bool getMeasStatus();
  private:
    QComboBox* cellType;
    QCheckBox* stimNode;
    QCheckBox* measNode;
    map<string, CellInitializer> cellMap;
    Node* node;
    cellInfo* info;
    gridCell* parentCell;
    bool stimStatus;
    bool measStatus;
  protected:
    void paintEvent(QPaintEvent *);
  public slots:
    void changeCell(QString);
  signals:
    void stimNodeChanged(int);
    void measNodeChanged(int);
    void cell_type_changed(QString);
};

class gridSetupWidget : public QWidget {
Q_OBJECT
  public:
	gridSetupWidget(QWidget* parent = 0);
    gridSetupWidget(gridProtocol* initial_proto, QDir workingDir, QWidget* parent = 0);
    ~gridSetupWidget() {}
    void setGrid(Grid* grid);
    Grid* getGrid();
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
    void changeStimNodeList(int status, pair<int,int> node);
    void changeMeasNodeList(int status, pair<int,int> node);
    void changeCellGroup(QString type);
    void toggleMeasurePressed();
    void toggleStimPressed();
  public slots:
    void updateMenu();
};
#endif
