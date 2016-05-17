#ifndef gridSetupWidget_H
#define gridSetupWidget_H

#include <QWidget>
#include <QDir>
#include <QTableWidget>
#include <QPushButton>
#include <QComboBox>
#include <QCheckBox>

#include "protocol.h"
#include "node.h"
#include "grid.h"

using namespace std;

class gridNode : public QWidget {
Q_OBJECT
  public:
    gridNode(Node* node);
    ~gridNode() {}
    Node* node();
  private:
    QComboBox* cellType;
    QCheckBox* stimNode;
    QCheckBox* measNode;
    Node* node;
    map<string, CellInitializer> cellMap;
  private slots:
    void changeCell(QString);
  signals:
    void stimNodeChanged(int status);
    void measNodeChanged(int status);
    void cell_type_changed();
}

class gridSetupWidget : public QWidget {
Q_OBJECT
  public:
    gridSettupWidget(Protocol* initial_proto, QDir workingDir, QWidget* parent = 0);
    ~gridSetupWidget() {}
    void setGrid(Grid* grid);
    Grid* getGrid();
  signals:
    void cell_type_changed();
  private:
    void createMenu();
    void updateMenu();
    
    Protocol* proto;
    QDir workingDir;
    QWidget* parent;
    Grid* grid;

    QTableWidget* cellGrid;
    QPushButton* addColumnButton;
    QPushButton* addRowButton;
  private slots:
    void changeStimNodeList(int status, Node* node);
    void changeMeasNodeList(int status, Node* node);
    void addRow();
    void addColumn();
}
#define
