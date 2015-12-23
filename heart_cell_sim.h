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
//also set buttons to enabled
    void set_sim_ready();
    void set_cell_ready();
    void init_douts();
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
    QPushButton* init_cell_button;
    QComboBox* cell_type;
    QComboBox* cell_species;

  private slots:
    void run_sims();
    void edit_simvars();
    void load_simvars();
    void edit_pvars();
    void load_pvars();
    void edit_dvars();
    void load_dvars();
    void edit_mvars();
    void load_mvars();
    void init_cell();
    void set_num_sims(int value);
};

#endif // HEART_CELL_SIM_H
