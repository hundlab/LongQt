/*
 * Measurement variables are the cell->vars which will have basic measurements
 * taken of them durring the simulation
 * This widget alows for them to be added and removed from the mvars list
 */
#ifndef MVARMENU_H
#define MVARMENU_H

#include <QWidget>
#include <QGridLayout>
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

using namespace std;

class mvarMenu :public QWidget {
Q_OBJECT
  public:
    mvarMenu(Protocol* initial_proto, QWidget* parent = 0);
    void createMenu();
   ~mvarMenu();

  private:
    Protocol* proto;
    QWidget* parent;
    set<string> measure_options;
    QMap<QString,QString> dvarsDescriptions; 
    QMap<QString,QString> measDescriptions;
//Buttons & their labels
    QCheckBox* set_vars;
    QPushButton* get_vars;
    QPushButton* close_button;
    QListWidget* vars_view;
    QListWidget* meas_view;
    QLabel* meas_list_label;
    QLabel* vars_list_label;
    QComboBox* addto_vars_options;
    QComboBox* addto_meas_options;
    QPushButton* addto_meas_list_button;
    QPushButton* removefr_meas_list_button;
    QPushButton* addto_vars_list_button;
    QPushButton* removefr_vars_list_button;
    QSpinBox* percrepol_spinbox;
//screen functions
    void update_menu(int row); //make menu match pars

  private slots:
    void addto_meas_list(); //add an item to mvnames
    void removefr_meas_list(); //remove and item from mvnames
    void addto_vars_list(); //add an item to mpnames
    void removefr_vars_list(); //remove and item from mpnames & its mvnames
    void switch_var(int row);
    void checkMeasOpts(const QString& value);
  public slots:
    void changeProto(Protocol* proto);
    void reset();
};

#endif
