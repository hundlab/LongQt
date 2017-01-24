#ifndef DVARMENU_H
#define DVARMENU_H

#include <QWidget>
#include <QGridLayout>
#include <QDir>
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

class dvarMenu :public QWidget {
Q_OBJECT
  public:
    dvarMenu(Protocol* initial_proto, QDir working_dir, QWidget* parent = 0);
    void createMenu();
    ~dvarMenu();
    void write_file();

  private:
    Protocol* proto;
    QWidget* parent;
    QDir working_dir;

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
    void update_datamap(string p, int state); //make a Protocol::pars entry match the screen
    void set_write_close(int state); //update function for write_close
  public slots:
    void changeProto(Protocol* proto);
    void reset();
    void setWorkingDir(QDir& dir);

};

#endif
