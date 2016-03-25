#include <QMainWindow>
#include <QGridLayout>
#include <QButtonGroup>
#include <QHBoxLayout>
#include <QPushButton>
#include <QComboBox>
#include <QSpinBox>
#include <QLabel>
#include <QFutureWatcher>
#include <QtConcurrent>
#include <QDir>
#include <QDate>
#include <QGroupBox>
#include <iterator>
#include <QProgressBar>
#include <QFileDialog>
#include <QStandardPaths>

#include "protocol.h"
#include "heart_cell_sim.h"
#include "varmenu.h"
#include "dialog.h"
#include "dvars_menu_object.cpp"
#include "pvars_menu_object.cpp"
#include "simvars_menu_object.cpp"
#include "mvars_menu_object.cpp"
#include "graph_menu_object.cpp"
#include "run_menu_object.cpp"

Simulation::Simulation(QWidget* parent){
//setup class variables
    this->parent = parent;
    proto = new Protocol();
    date_time = QDate::currentDate().toString("MMddyy") + "-" + QTime::currentTime().toString("hhmm");
    working_dir = QStandardPaths::standardLocations(QStandardPaths::DocumentsLocation).first() + "/data" + date_time;
    proto->datadir = working_dir.absolutePath().toStdString();
    menu_list = new QList<menu_object*>();
//create layouts
    main_layout = new QGridLayout(this);
//organizational widgets
    menu = new QStackedWidget();
    menu_options = new QListWidget();
//create buttons/combo boxes
    run_button = new QPushButton("Run Simulations");
    next_button = new QPushButton("Next");
    cancel_button = new QPushButton("Cancel");
//add items menu_list
    menu_list->append(new simvars_menu_object("Edit Simvars",proto,working_dir, this));
    menu_list->append(new dvars_menu_object("Edit DVars", proto,working_dir));
    menu_list->append(new mvars_menu_object("Edit MVars", proto,working_dir));
    menu_list->append(new pvars_menu_object("Edit PVars", proto,working_dir));
    menu_list->append(run_menu_object("Run Simulation"));
//set button/combo box inital values
    cancel_button->hide();
//menu
    for(auto it = menu_list->begin(); it != menu_list->end(); it++) {
        menu_options->addItem((*it)->name);
        menu->addWidget((*it)->getWidget());
    }
//main_layout
    main_layout->addWidget(menu_options, 0,0,-1,1);
    main_layout->addWidget(menu, 0, 1);
    main_layout->addWidget(next_button, 1, 2);
    main_layout->addWidget(cancel_button, 1, 2);
    QSizePolicy sPol(QSizePolicy::Minimum, QSizePolicy::Expanding);
    menu_options->setSizePolicy(sPol);
    menu_options->setWindowTitle(date_time);
//connect buttons
    connect(run_button, SIGNAL(clicked()),this,SLOT(run_sims()));
    connect(menu_options, SIGNAL(currentRowChanged(int)), this, SLOT(list_click_aciton(int)));
    connect(next_button, SIGNAL(clicked()), this, SLOT(next_button_aciton()));
    connect((simvarMenu*)menu_list->first()->getWidget(), SIGNAL(cell_type_changed()), this, SLOT(cell_changed()));
    connect((simvarMenu*)menu_list->first()->getWidget(), SIGNAL(working_dir_changed(QDir)), this, SLOT(change_working_dir(QDir)));
};
Simulation::~Simulation(){};
void Simulation::cell_changed() {
    menu_list[1]->reset();
    menu_list[2]->reset();
    menu_list[3]->reset();
void Simulation::change_working_dir(QDir dir) {
    this->working_dir = dir;
    menu_list[1]->setWorkingDir();
    menu_list[2]->setWorkingDir();
    menu_list[3]->setWorkingDir();
};
void Simulation::run_sims() {
    int i = 0;
    Protocol* temp;
    next_button->hide();
    cancel_button->show();
    run_button->setEnabled(false);
    working_dir.mkpath(working_dir.absolutePath());

    for( i = 0; i < proto->numtrials; i++) {
        proto->setTrial(i);
        temp = new Protocol(*proto);
        temp->readfile = "/r"+ to_string(i) + ".dat"; // File to read SV ICs
        temp->savefile = "/s"+ to_string(i) + ".dat"; // File to save final SV
        temp->propertyoutfile = "/dt%d_%s" + string(".dat");
        temp->dvarsoutfile = "/dt%d_dvars" + string(".dat");
        temp->finalpropertyoutfile = "/dss%d_%s" + string(".dat");
        temp->finaldvarsoutfile = "/dss%d_pvars" + string(".dat");
        vector.append(*temp);
    } 

 
    connect(cancel_button,SIGNAL(clicked()),&watcher,SLOT(cancel()));  // connect signals and slots for watcher and dialog
    connect(cancel_button,SIGNAL(clicked()),this,SLOT(canceled()));
    connect(&watcher,SIGNAL(finished()),this,SLOT(finished()));
    connect(&watcher,SIGNAL(finished()),pdialog,SLOT(reset()));
    connect(&watcher,SIGNAL(progressRangeChanged(int,int)),pdialog,SLOT(setRange(int,int)));
    connect(&watcher,SIGNAL(progressValueChanged(int)),pdialog,SLOT(setValue(int)));
    
    QFuture<void> next = QtConcurrent::map(vector,[] (Protocol& p) {p.runTrial();});  // pass vector of protocols to QtConcurrent

    watcher.setFuture(next);

};
void Simulation::list_click_aciton (int next_row) {
    menu_list[menu->currentIndex()]->leave();
    menu->setCurrentIndex(next_row);
    menu_options->setCurrentRow(next_row);
}
void Simulation::next_button_aciton () {
    int current_row = menu->currentIndex();
    leave_current(current_row);
    if(menu->count() > current_row +1) {
        menu->setCurrentIndex(current_row +1);
        menu_options->setCurrentRow(current_row +1);
    }
}
void Simulation::canceled() {
    qDebug()<<"canceled!";
    watcher.waitForFinished();
    QMessageBox::critical(this,"Cancel","Simulation canceled!");
    vector.clear();
    cancel_button->hide();
    next_button->show();
    run_button->setEnabled(false);
}
void Simulation::finished() {
    qDebug()<<"finished!";
    QMessageBox::information(this,"Finish","Simulation finished!");
    menu_list->append(new graph_menu_object(proto, working_dir, this));
    menu->addWidget(menu_list->last()->getWidget());
    date_time = QDate::currentDate().toString("MMddyy") + "-" + QTime::currentTime().toString("hhmm");
    working_dir.cdUp();
//    working_dir.mkdir("./data" + date_time);
    working_dir.cd("./data" + date_time);
    change_working_dir(working_dir);
    cancel_button->hide();
    next_button->show();
    run_button->setEnabled(false);
}
