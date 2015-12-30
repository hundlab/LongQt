#ifndef VARMENU_H
#define VARMENU_H

#include <QWidget>
#include <QDebug>
#include <QMessageBox>
#include <QDoubleSpinBox>
#include <QSignalMapper>

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
    Protocol proto;
    QWidget* parent;
    bool write_close;
//Buttons & their labels
    QDoubleSpinBox** simvars;
    QLabel** simvar_names;
    QCheckBox* set_vars;
    QPushButton* get_vars;
    QPushButton* close_button;
//Map from button output to signal input
    QSignalMapper* simvar_map;
//screen functions
    void update_menu(); //make menu match pars

  private slots:
    bool read_simvars(); //wrapper for Protocol::readpars with QFileDialog
    bool write_simvars(); //wrapper for Protocol::writepars
    void update_pvars(int id); //make a Protocol::pars entry match the screen
    void set_write_close(int state); //update function for write_close
};

//class pvarMenu :public QWidget {};

class dvarMenu :public QWidget {
Q_OBJECT
  public:
    dvarMenu(Protocol* initial_proto, QWidget* parent = 0);
    ~dvarMenu();


  private:
    Protocol proto;
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

//class mvarMenu :public QWidget {};
#endif // VARMENU_H
