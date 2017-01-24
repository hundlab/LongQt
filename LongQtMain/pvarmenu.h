#ifndef PVARMENU_H
#define PVARMENU_H

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

#endif
