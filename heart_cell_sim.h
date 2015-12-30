#ifndef HEART_CELL_SIM_H
#define HEART_CELL_SIM_H

#include <QPushButton>
#include <QLabel>
#include <QSpinBox>
#include <QComboBox>
#include "proto.h"

class Simulation : public QWidget {
    Q_OBJECT

  public:
    Simulation(QWidget* parent = 0);
    ~Simulation();

  private:
    QWidget* parent;
    Protocol* proto;
    unsigned int num_sims;
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
//buttons
    QPushButton* run_button;
    QSpinBox* num_of_sims;
    QLabel* num_of_sims_label;
    QPushButton* edit_sim_button;
    QPushButton* load_sim_button;
    QPushButton* edit_pvars_button;
    QPushButton* load_pvars_button;
    QPushButton* edit_dvars_button;
    QPushButton* load_dvars_button;
    QPushButton* edit_mvars_button;
    QPushButton* load_mvars_button;
    QPushButton* load_all_button;
    QPushButton* init_cell_button;
    QComboBox* cell_type;
    QComboBox* cell_species;

  private slots:
    void run_sims();//action for running the simulation
    void edit_simvars();//open simvarMenu
    void load_simvars();//wrapper for proto->readpars
    void edit_pvars();//open pvarsMenu
    void load_pvars();//wrapper for proto->readpvars
    void edit_dvars();//open dvarsMenu
    void load_dvars();//wrapper for proto->resizemap
    void edit_mvars();//open mvarsMenu
    void load_mvars();//wrapper for proto->initializeMeasure
    void init_cell();//initialize proto->cell
    void set_num_sims(int value);//make num_sims match num_of_sims
};

#endif // HEART_CELL_SIM_H
