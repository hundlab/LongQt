#ifndef gridSetupWidget_H
#define gridSetupWidget_H

#include <QWidget>
#include <QDir>
#include <QTableWidget>
#include <QPushButton>
#include <QComboBox>
#include <QCheckBox>

#include "gridProtocol.h"
#include "node.h"
#include "grid.h"
#include "gridCell.h"

using namespace std;

class gridNode : public QWidget {
Q_OBJECT
  public:
    gridNode(Node* node, int X, int Y, gridCell* parentCell);
    ~gridNode();
    Node* getNode();
    pair<int,int> getNodePair();
    void update(bool stim, bool meas);
  private:
    QComboBox* cellType;
    QCheckBox* stimNode;
    QCheckBox* measNode;
    map<string, CellInitializer> cellMap;
    Node* node;
    cellInfo* info;
    gridCell* parentCell;
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

    QTableWidget* cellGrid;
    QPushButton* addColumnButton;
    QPushButton* removeColumnButton;
    QPushButton* addRowButton;
    QPushButton* removeRowButton;
  private slots:
    void changeStimNodeList(int status, pair<int,int> node);
    void changeMeasNodeList(int status, pair<int,int> node);
    void addRow();
    void addColumn();
    void removeRow();
    void removeColumn();
    void changeCellGroup(QString type);
  public slots:
    void updateMenu();
};
#endif