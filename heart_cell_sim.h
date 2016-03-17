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

#include "protocol.h"
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
    QFutureWatcher<void> watcher;
    QFuture<void> next;
    QVector<Protocol> vector;
    QList<std::tuple<QString,bool,QWidget*>>* menu_list;
//utility functions
    void leave_current(int current);
//buttons
    QPushButton* run_button;
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

  private slots:
    void cell_changed();
    void run_sims();//action for running the simulation
    void next_button_aciton();
    void list_click_aciton (int next_row);
    void canceled();
    void finished();
};

#endif // HEART_CELL_SIM_H
