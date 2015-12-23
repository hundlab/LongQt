#ifndef HEART_CELL_SIM_H
#define HEART_CELL_SIM_H

#include <QMainWindow>

#include "proto.h"

class Simulation : public QWidget {
    Q_OBJECT

  public:
    Simulation(QWidget* parent = 0);
    ~Simulation();

  private:
    QWidget* parent;
    Protocol* proto;
//booleans for button availabilites
    bool sim_ready;
//boolean manipulation functions
//    void set_sim_ready();

  private slots:
    void run_sims();
    void edit_simvars();
    bool load_simvars();
};

#endif // HEART_CELL_SIM_H
