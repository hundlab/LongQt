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
    simvarMenu(Protocol* initial_proto, QWidget* parent = 0);
    ~simvarMenu();

  protected:
    void closeEvent(QCloseEvent* event);

  private:
    Protocol proto;
    QWidget* parent;
    bool write_close;
    QDoubleSpinBox** simvars;
    QLabel** simvar_names;
    QSignalMapper* simvar_map;
    QCheckBox* set_vars;
    QPushButton* get_vars;
    QPushButton* close_button;
    void update_menu();

  private slots:
    bool read_simvars();
    bool write_simvars();
    void update_pvars(int id);
    void set_write_close(int state);
};
#endif // VARMENU_H
