#ifndef HEART_CELL_SIM_H
#define HEART_CELL_SIM_H

#include <QPushButton>
#include <QLabel>
#include <QSpinBox>
#include <QComboBox>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QStackedWidget>
#include <QListWidget>
#include <QList>
#include <QProgressBar>
#include <QFuture>
#include <QFutureWatcher>

#include "proto.h"
#include "varmenu.h"

class Simulation : public QWidget {
    Q_OBJECT

  public:
    Simulation(QWidget* parent = 0);
    ~Simulation();

  private:
    QWidget* parent;
    Protocol* proto;
    QString date_time;
    unsigned int num_sims;
    QFutureWatcher<void> watcher;
    QFuture<void> next;
    QVector<Protocol> vector;
    QList<std::tuple<QString,bool,QWidget*>>* menu_list;
//booleans for button availabilites
    bool sim_ready;
    bool cell_ready;
    bool simvars_read;
    bool pvars_read;
    bool mvars_read;
    bool dvars_read;
    bool douts_ready;
//boolean manipulation functions
//also set buttons to match booleans
    void set_sim_ready();
    void set_cell_ready();
    void init_douts();//initialize proto->douts when ready
//utility functions
    static void doTask(Protocol& toRun);//run the simulaiton
    void leave_current(int current);
//buttons
    QPushButton* run_button;
    QSpinBox* num_of_sims;
    QLabel* num_of_sims_label;
    QPushButton* load_sim_button;
    QPushButton* load_pvars_button;
    QPushButton* load_dvars_button;
    QPushButton* load_mvars_button;
    QPushButton* load_all_button;
    QPushButton* init_cell_button;
    QComboBox* cell_type;
    QComboBox* cell_species;
    QProgressBar* pdialog;
    QPushButton* next_button;
    QPushButton* cancel_button;
//organizational widgets
    QListWidget* menu_options;
    QStackedWidget* menu;
//varmenu widgets
    simvarMenu* edit_simvars_menu;
    dvarMenu* edit_dvars_menu;
    mvarMenu* edit_mvars_menu;
    pvarMenu* edit_pvars_menu;
//layouts
    QGridLayout* main_layout;
    QHBoxLayout* file_buttons;
    QHBoxLayout* cell_buttons;

  private slots:
    void run_sims();//action for running the simulation
    void load_simvars();//wrapper for proto->readpars
    void load_pvars();//wrapper for proto->readpvars
    void load_dvars();//wrapper for proto->resizemap
    void load_mvars();//wrapper for proto->initializeMeasure
    void init_cell();//initialize proto->cell
    void set_num_sims(int value);//make num_sims match num_of_sims
    void next_button_aciton();
    void list_click_aciton (int next_row);
    void canceled();
    void finished();
};

#endif // HEART_CELL_SIM_H
