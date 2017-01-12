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
#include <QLineEdit>
#include <QFormLayout>
#include <QMap>
#include <QDir>
#include <set>

#include "protocol.h"
#include "gridSettup.h"

using namespace std;

//
class simvarMenu :public QWidget {
Q_OBJECT
  public:
    simvarMenu(Protocol* initial_proto, QDir working_dir, QWidget* parent = 0);
    void createMenu();
    ~simvarMenu();
    void write_file();

  protected:
    void closeEvent(QCloseEvent* event);

  private:
    Protocol* proto;
    QWidget* parent;
    QDir working_dir;
    bool write_close;
//Buttons & their labels
    QGridLayout* main_layout;
    QMap<QString, QString> descriptions;
    QMap<QString, QWidget*> simvars;
    QMap<QString,QFormLayout*> simvars_layouts;
    QCheckBox* set_vars;
    QPushButton* get_vars;
    QPushButton* close_button;
	gridSetupWidget* grid = 0;
    void set_default_vals(string name);
//initalizer
    void initialize(const map<string,GetSetRef>::iterator it);
//screen functions
    void update_menu(); //make menu match pars
	bool signalCellTypeChange = true;

  private slots:
    bool read_simvars(); //wrapper for Protocol::readpars with QFileDialog
    bool write_simvars(); //wrapper for Protocol::writepars
    void update_pvars(pair<string, double> p); //make a Protocol::pars entry match the screen
    void update_pvars(pair<string, string> p, string type);
    void update_pvars(pair<string, int> p, string type = "int");
    void set_write_close(int state); //update function for write_close
  public slots:
    void changeProto(Protocol* proto);
    void changeCellType();
    void reset();
    void setWorkingDir(QDir& dir);
  signals:
    void cell_type_changed();
    void working_dir_changed(QDir&);
    void updated();
};

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

class mvarMenu :public QWidget {
Q_OBJECT
  public:
    mvarMenu(Protocol* initial_proto, QDir working_dir, QWidget* parent = 0);
    void createMenu();
   ~mvarMenu();
    void write_file();

  private:
    Protocol* proto;
    QWidget* parent;
    QDir working_dir;
    bool write_close;
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
    void checkMeasOpts(const QString& value);
  public slots:
    void changeProto(Protocol* proto);
    void reset();
    void setWorkingDir(QDir& dir);
};

class pvarMenu :public QWidget {
Q_OBJECT
  public:
    pvarMenu(Protocol* initial_proto, QDir working_dir, QWidget* parent = 0);
    void createMenu();
    ~pvarMenu();
    void write_file();

  private:
    Protocol* proto;
    QWidget* parent;
    QDir working_dir;
    bool write_close;
    QMap<QString,QString> pvarsDescriptions;
//Buttons & their labels
    QGridLayout* central_layout;
    QCheckBox* set_vars;
    QPushButton* get_vars;
    QTableWidget* pvar_table;
    QPushButton* close_button;
    QPushButton* add_button;
    QComboBox* new_var_choice;
//Lists for pvals options
    QStringList* pvals_options;
//screen functions
    void update_menu(); //make menu match pvars
    void update_menu(unsigned int row);
    void clear_row(unsigned int row, int offset);
    void add_doublespinbox_tomenu(unsigned int row, unsigned int column, unsigned int boxlen = 1, int column_pos = -1);
    void add_comobobox_tomenu(unsigned int row, unsigned int column, unsigned int menu_box, unsigned int boxlen = 1, int column_pos = -1);

  protected:
    void closeEvent(QCloseEvent* event);

  private slots:
    bool read_pvars(); 
    bool write_pvars(); 
    void set_write_close(int state); //update function for write_close
    void row_changed(QString value, int row, int column);
    void remove_row(unsigned int row);
    void add_row();
    void checkbox_changed(unsigned int row,unsigned int column,int state);
  public slots:
    void changeProto(Protocol* proto);
    void reset();
    void setWorkingDir(QDir& dir);
};

#endif // VARMENU_H
