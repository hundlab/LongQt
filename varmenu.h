#ifndef VARMENU_H
#define VARMENU_H

#include <QWidget>
#include <QDebug>
#include <QMessageBox>
#include <QDoubleSpinBox>
#include <QSignalMapper>
#include <QListWidget>
#include <QVector>
#include <QStringList>
#include <QComboBox>
#include <QTableWidget>
#include <QGridLayout>

#include "proto.h"

using namespace std;

//
class simvarMenu :public QWidget {
Q_OBJECT
  public:
    simvarMenu(Protocol* initial_proto, QWidget* parent = 0);
    ~simvarMenu();

  protected:
    void closeEvent(QCloseEvent* event);

  private:
    Protocol* proto;
    QWidget* parent;
    bool write_close;
//Buttons & their labels
    QDoubleSpinBox** simvars;
    QLabel** simvar_names;
    QCheckBox* set_vars;
    QPushButton* get_vars;
    QPushButton* close_button;
//screen functions
    void update_menu(); //make menu match pars

  private slots:
    bool read_simvars(); //wrapper for Protocol::readpars with QFileDialog
    bool write_simvars(); //wrapper for Protocol::writepars
    void update_pvars(pair<string, double> p); //make a Protocol::pars entry match the screen
    void set_write_close(int state); //update function for write_close
};

class dvarMenu :public QWidget {
Q_OBJECT
  public:
    dvarMenu(Protocol* initial_proto, QWidget* parent = 0);
    ~dvarMenu();


  private:
    Protocol* proto;
    QWidget* parent;
    bool write_close;
//Buttons & their labels
    QCheckBox** dvars;
    QCheckBox* set_vars;
    QPushButton* get_vars;
    QPushButton* close_button;
//screen functions
    void update_menu(); //make menu match pars

  protected:
    void closeEvent(QCloseEvent* event);

  private slots:
    bool read_dvars(); //wrapper for Protocol::readpars with QFileDialog
    bool write_dvars(); //wrapper for Protocol::writepars
    void update_datamap(pair<string,double*> p, int state); //make a Protocol::pars entry match the screen
    void set_write_close(int state); //update function for write_close

};

class mvarMenu :public QWidget {
Q_OBJECT
  public:
    mvarMenu(Protocol* initial_proto, QWidget* parent = 0);
    ~mvarMenu();

  private:
    Protocol* proto;
    QWidget* parent;
    bool write_close;
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
//screen functions
    void update_menu(int row); //make menu match pars

  protected:
    void closeEvent(QCloseEvent* event);

  private slots:
    bool read_mvars(); //wrapper for Protocol::readpars with QFileDialog
    bool write_mvars(); //wrapper for Protocol::writepars
    void set_write_close(int state); //update function for write_close
    void addto_meas_list(); //add an item to mvnames
    void removefr_meas_list(); //remove and item from mvnames
    void addto_vars_list(); //add an item to mpnames
    void removefr_vars_list(); //remove and item from mpnames & its mvnames
    void switch_var(int row);
;
};

class pvarMenu :public QWidget {
Q_OBJECT
  public:
    pvarMenu(Protocol* initial_proto, QWidget* parent = 0);
    ~pvarMenu();

  private:
    Protocol* proto;
    QWidget* parent;
    bool write_close;
//Buttons & their labels
    QGridLayout* central_layout;
    QCheckBox* set_vars;
    QPushButton* get_vars;
    QTableWidget* pvar_table;
    QPushButton* close_button;
//Lists for pvals options
    QStringList* pvals_options;
//screen functions
    void update_menu(); //make menu match pvars
    void update_menu(unsigned int row);

  protected:
    void closeEvent(QCloseEvent* event);

  private slots:
    bool read_pvars(); 
    bool write_pvars(); 
    void set_write_close(int state); //update function for write_close
;
};

#endif // VARMENU_H
