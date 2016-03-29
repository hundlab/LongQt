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

Simulation::Simulation(QWidget* parent){
//setup class variables
    this->parent = parent;
    proto = new Protocol();
    date_time = QDate::currentDate().toString("MMddyy") + "-" + QTime::currentTime().toString("hhmm");
    working_dir = QStandardPaths::standardLocations(QStandardPaths::DocumentsLocation).first() + "/data" + date_time;
    proto->datadir = working_dir.absolutePath().toStdString();
    menu_list = new QList<tuple<QString,bool,QWidget*>>();
//create layouts
    main_layout = new QGridLayout(this);
//organizational widgets
    menu = new QStackedWidget();
    menu_options = new QListWidget();
//create buttons/combo boxes
    run_button = new QPushButton("Run Simulations");
    next_button = new QPushButton("Next");
    cancel_button = new QPushButton("Cancel");
    pdialog = new QProgressBar();
//setup run button container
    QWidget* run_button_container = new QWidget();
    QGridLayout* run_button_container_layout = new QGridLayout();
    run_button_container_layout->addWidget(run_button, 0,0,1,2);
    run_button_container_layout->addWidget(new QLabel("Progress"), 1,0);
    run_button_container_layout->addWidget(pdialog, 1,1);
    run_button_container->setLayout(run_button_container_layout);
//add items menu_list
    menu_list->append(std::tuple<QString,bool,QWidget*> ("Edit Simvars",true,new simvarMenu(proto,working_dir, this)));
    menu_list->append(std::tuple<QString,bool,QWidget*> ("Edit DVars",true, new dvarMenu(proto,working_dir)));
    menu_list->append(std::tuple<QString,bool,QWidget*> ("Edit MVars",true, new mvarMenu(proto,working_dir)));
    menu_list->append(std::tuple<QString,bool,QWidget*> ("Edit PVars",true, new pvarMenu(proto,working_dir)));
    menu_list->append(std::tuple<QString,bool,QWidget*> ("Run Simulation",true, run_button_container));
    menu_list->append(std::tuple<QString,bool,QWidget*> ("Graph",false, NULL));
//set button/combo box inital values
    cancel_button->hide();
//menu
    for(auto it = menu_list->begin(); it != menu_list->end(); it++) {
        menu_options->addItem(std::get<0>(*it));
        if(std::get<1>(*it)) {
            menu->addWidget(std::get<2>(*it));
        }
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
    connect(std::get<2>(menu_list->first()), SIGNAL(cell_type_changed()), this, SLOT(cell_changed()));
    connect(std::get<2>(menu_list->first()), SIGNAL(working_dir_changed(QDir)), this, SLOT(change_working_dir(QDir)));
};
Simulation::~Simulation(){};
void Simulation::cell_changed() {
    menu->removeWidget(std::get<2>(menu_list->at(1)));
    menu->removeWidget(std::get<2>(menu_list->at(2)));
    menu->removeWidget(std::get<2>(menu_list->at(3)));
    menu_list->replace(1, make_tuple(std::get<0>(menu_list->at(1)), true, new dvarMenu(proto,working_dir, this)));
    menu_list->replace(2, make_tuple(std::get<0>(menu_list->at(2)), true, new mvarMenu(proto,working_dir, this)));
    menu_list->replace(3, make_tuple(std::get<0>(menu_list->at(3)), true, new pvarMenu(proto,working_dir, this)));
    menu->insertWidget(1, std::get<2>(menu_list->at(1)));
    menu->insertWidget(2, std::get<2>(menu_list->at(2)));
    menu->insertWidget(3, std::get<2>(menu_list->at(3)));
};
void Simulation::change_working_dir(QDir dir) {
    this->working_dir = dir;
    ((dvarMenu*)std::get<2>(menu_list->at(1)))->setWorkingDir(dir);
    ((mvarMenu*)std::get<2>(menu_list->at(2)))->setWorkingDir(dir);
    ((pvarMenu*)std::get<2>(menu_list->at(3)))->setWorkingDir(dir);
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
    leave_current(menu->currentIndex());
    if(std::get<1>(menu_list->at(next_row))) {
        menu->setCurrentIndex(next_row);
        menu_options->setCurrentRow(next_row);
    } else {
        menu_options->setCurrentRow(menu->currentIndex());
    }

}
void Simulation::next_button_aciton () {
    int current_row = menu->currentIndex();
    leave_current(current_row);
    if(std::get<1>(menu_list->value(current_row +1))) {
        menu->setCurrentIndex(current_row +1);
        menu_options->setCurrentRow(current_row +1);
    }
}
void Simulation::leave_current(int current) {
    switch(current) {
    case 0:
        ((simvarMenu*) std::get<2>(menu_list->at(current)))->write_file();
    break;
    case 1:
        ((dvarMenu*) std::get<2>(menu_list->at(current)))->write_file();
    break;
    case 2:
        ((mvarMenu*) std::get<2>(menu_list->at(current)))->write_file();
    break;
    case 3:
        ((pvarMenu*) std::get<2>(menu_list->at(current)))->write_file();
    break;
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
    //qDebug()<<"finished!";
    //QMessageBox::information(this,"Finish","Simulation finished!");
    menu_list->replace(5, make_tuple(std::get<0>(menu_list->at(5)), true, new Dialog(proto, working_dir, this)));
    menu->insertWidget(5, std::get<2>(menu_list->at(5)));
    date_time = QDate::currentDate().toString("MMddyy") + "-" + QTime::currentTime().toString("hhmm");
    working_dir.cdUp();
//    working_dir.mkdir("./data" + date_time);
    working_dir.cd("./data" + date_time);
    change_working_dir(working_dir);
    cancel_button->hide();
    next_button->show();
    run_button->setEnabled(false);
}
