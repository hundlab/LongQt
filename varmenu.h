#ifndef VARMENU_H
#define VARMENU_H

#include <QWidget>
#include <QDebug>
#include <QMessageBox>
#include <QDoubleSpinBox>
#include <QSignalMapper>

#include "proto.h"

using namespace std;

class simvarMenu :public QWidget {
Q_OBJECT
  public:
    simvarMenu(QWidget * parent, Protocol initial_proto);
    ~simvarMenu();
  private:
    void update_menu();
    Protocol proto;
    QWidget* parent;
    QDoubleSpinBox** simvars;
    QLabel** simvar_names;
    QSignalMapper* simvar_map;

  private slots:
    bool read_simvars();
    bool write_simvars();
    void update_pvars(int id);
};
#endif // VARMENU_H
