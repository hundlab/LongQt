/*
 * sim variables are in proto->pars and control how the simulation is run
 * this is a widget to edit those variables
 */
#ifndef SIMVARMENU_H
#define SIMVARMENU_H

#include <QWidget>
#include <QGridLayout>
#include <QFormLayout>
#include <QDir>
#include <QMap>
#include <QDebug>
#include <QMessageBox>
#include <QDoubleSpinBox>
#include <QSignalMapper>
#include <QListWidget>
#include <QVector>
#include <QStringList>
#include <QComboBox>
#include <QTableWidget>
#include <QLineEdit>
#include <set>

#include "protocol.h"
#include "gridsetupwidget.h"

using namespace std;

class simvarMenu :public QWidget {
Q_OBJECT
  public:
    simvarMenu(Protocol* initial_proto, QWidget* parent = 0);
    void createMenu();
    ~simvarMenu();

  private:
    Protocol* proto;
//Buttons & their labels
    QGridLayout* main_layout;
    QMap<QString, QString> descriptions;
    QMap<QString, QWidget*> simvars;
    QMap<QString,QFormLayout*> simvars_layouts;
	GridSetupWidget* grid = 0;
//initalizer
    void initialize(const map<string,GetSetRef>::iterator it);
//screen functions
    void update_menu(); //make menu match pars
	bool signalCellTypeChange = true;

  private slots:
    void update_pvars(pair<string, double> p); //make a Protocol::pars entry match the screen
    void update_pvars(pair<string, string> p, string type);
    void update_pvars(pair<string, int> p, string type = "int");
  public slots:
    void changeProto(Protocol* proto);
    void changeCell(Cell*);
    void reset();
    void setWorkingDir(QDir& dir);
  signals:
    void cellChanged(Cell*);
    void updated();
};

#endif
