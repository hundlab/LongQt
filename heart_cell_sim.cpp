#include <QMainWindow>
#include <QGridLayout>
#include <QButtonGroup>
#include <QHBoxLayout>
#include <QPushButton>
#include <QComboBox>
#include <QSpinBox>
#include <QLabel>
#include <QProgressDialog>
#include <QFutureWatcher>
#include <QtConcurrent>
#include <QDir>
#include <QDate>

#include "proto.h"
#include "heart_cell_sim.h"
#include "varmenu.h"
#include "dialog.h"
QString Dinf;
QString Tinf;
Simulation::Simulation(QWidget* parent){
//setup class variables
    this->parent = parent;
    proto = new Protocol();
    simvars_read = false;
    pvars_read = false;
    mvars_read = false;
    dvars_read = false;
    douts_ready = false;
    cell_ready = false;
    sim_ready = false;
    num_sims = 1;
//change the location of proto readin files
//    proto->simvarfile = "data/simvars.txt";
//    proto->dvarfile = "data/dvars.txt";
//    proto->pvarfile = "data/pvars.txt";
//local variables
    unsigned int max_height = 8;
    unsigned int max_width = 14;
//create layouts
    QGridLayout* main_layout = new QGridLayout(this);
    QGridLayout* advanced = new QGridLayout;
    QHBoxLayout* file_buttons = new QHBoxLayout;
//create buttons/combo boxes
    num_of_sims = new QSpinBox(this);
    num_of_sims_label = new QLabel("Number of Simulations:", this);
    run_button = new QPushButton("Run Simulations", this);
    edit_sim_button = new QPushButton("Edit Simulaiton Variables", this);
    load_sim_button = new QPushButton("Read Simulation Variables");
    edit_pvars_button = new QPushButton("Edit Simulation Constants", this);
    load_pvars_button = new QPushButton("Read Simulaiton Constants");
    edit_dvars_button = new QPushButton("Edit Output Variables", this);
    load_dvars_button = new QPushButton("Read Output Variables");
    edit_mvars_button = new QPushButton("Edit Measurement Variables", this);
    load_mvars_button = new QPushButton("Read Measurement Variables");
    load_all_button = new QPushButton("Read Variables and Constants");
    init_cell_button = new QPushButton("Use cell preference", this);
    cell_type = new QComboBox(this);
    cell_species = new QComboBox(this);
//set button/combo box inital values
    num_of_sims->setValue(num_sims);
    run_button->setEnabled(sim_ready);
    load_pvars_button->setEnabled(cell_ready);
    edit_pvars_button->setEnabled(cell_ready);
    load_dvars_button->setEnabled(cell_ready);
    edit_dvars_button->setEnabled(cell_ready);
    load_all_button->setEnabled(cell_ready);
    cell_type->addItem("Default Cell");
    cell_species->addItem("Default Species");
//add buttons to layouts
//advanced buttons
    advanced->addWidget(edit_sim_button, 0,0);
    advanced->addWidget(edit_pvars_button,0,1);
    advanced->addWidget(edit_dvars_button,0,2);
    advanced->addWidget(edit_mvars_button,0,3);
//load variables buttons
//    file_buttons->addWidget(load_sim_button);
//    file_buttons->addWidget(load_pvars_button);
//    file_buttons->addWidget(load_dvars_button);
//    file_buttons->addWidget(load_mvars_button);
    file_buttons->addWidget(load_all_button);
//main_layout
    main_layout->addWidget(cell_type, 0,0,1,1);
    main_layout->addWidget(cell_species, 0,1,1,1);
    main_layout->addWidget(init_cell_button,0,2,1,1);
    main_layout->addWidget(num_of_sims_label, 0,3,1,1);
    main_layout->addWidget(num_of_sims,0,4,1,1);
    main_layout->addLayout(file_buttons,1,0,1,5);
    main_layout->addLayout(advanced, max_height -1, 0,1,4);
    main_layout->addWidget(run_button, max_height, max_width);
//connect buttons
    connect(run_button, SIGNAL(clicked()),this,SLOT(run_sims()));
    connect(edit_sim_button, SIGNAL(clicked()), this, SLOT(edit_simvars()));
    connect(load_sim_button, SIGNAL(clicked()),this, SLOT(load_simvars()));
    connect(edit_pvars_button, SIGNAL(clicked()), this, SLOT(edit_pvars()));
    connect(load_pvars_button, SIGNAL(clicked()), this, SLOT(load_pvars()));
    connect(edit_dvars_button, SIGNAL(clicked()), this, SLOT(edit_dvars()));
    connect(load_dvars_button, SIGNAL(clicked()), this, SLOT(load_dvars()));
    connect(edit_mvars_button, SIGNAL(clicked()), this, SLOT(edit_mvars()));
    connect(load_mvars_button, SIGNAL(clicked()), this, SLOT(load_mvars()));
    connect(load_all_button, SIGNAL(clicked()), this, SLOT(load_simvars()));
    connect(load_all_button, SIGNAL(clicked()), this, SLOT(load_pvars()));
    connect(load_all_button, SIGNAL(clicked()), this, SLOT(load_dvars()));
    connect(load_all_button, SIGNAL(clicked()), this, SLOT(load_mvars()));
    connect(init_cell_button, SIGNAL(clicked()), this, SLOT(init_cell()));
    connect(num_of_sims, SIGNAL(valueChanged(int)), this, SLOT(set_num_sims(int)));
};

Simulation::~Simulation(){};

void Simulation::set_sim_ready() {
    sim_ready = simvars_read && pvars_read && mvars_read && dvars_read && cell_ready && douts_ready;
    run_button->setEnabled(sim_ready);
};

void Simulation::set_cell_ready() {
    cell_ready = true;
    load_pvars_button->setEnabled(cell_ready);
    edit_pvars_button->setEnabled(cell_ready);
    load_dvars_button->setEnabled(cell_ready);
    edit_dvars_button->setEnabled(cell_ready);
    load_all_button->setEnabled(cell_ready);
};

void Simulation::init_douts() {
    if(simvars_read&&mvars_read) {
        proto->douts = new Output[proto->getNeededDOutputSize()]();   // Array of i/o data streams
    douts_ready = true;
    }
};

void Simulation::doTask(Protocol& toRun) {
    toRun.runSim(); 
};

void Simulation::run_sims() {
    unsigned int i = 0;
    Protocol* temp;
    QVector<Protocol> vector;
    //QDir().mkdir("data" + QDate::currentDate().toString("MMddyy"));
     Dinf = "data" +QDate::currentDate().toString("MMddyy");
     Tinf = QTime::currentTime().toString("hm");
    QDir().mkdir(Dinf+"-"+Tinf);
   for( i = 0; i < num_sims; i++) {
        temp = new Protocol(*proto);
        temp->readfile = "./data" + QDate::currentDate().toString("MMddyy").toStdString() + "/r"+ to_string(i) + ".dat"; // File to read SV ICs
        temp->savefile = "./data" + QDate::currentDate().toString("MMddyy").toStdString() + "/s"+ to_string(i) + ".dat"; // File to save final SV
        temp->propertyoutfile = "./" + Dinf.toStdString()+"-" + Tinf.toStdString() + "/dt%d_%s" + to_string(i) + string(".dat");
        temp->dvarsoutfile = "./" + Dinf.toStdString() + "-" + Tinf.toStdString() + "/dt%d_dvars"+ to_string(i) + string(".dat");
        temp->finalpropertyoutfile = "./" + Dinf.toStdString() + "-" + Tinf.toStdString() + "/dss_%s"+ to_string(i) + string(".dat");
        temp->finaldvarsoutfile = "./" + Dinf.toStdString() + "-" + Tinf.toStdString() + "/dss_pvars"+ to_string(i) + string(".dat");
        vector.append(*temp);
  }

    QProgressDialog pdialog;
    pdialog.setLabelText("Processing");
    QFutureWatcher<void> watcher;

    connect(&pdialog,SIGNAL(canceled()),&watcher,SLOT(cancel()));  // connect signals and slots for watcher and dialog
    connect(&watcher,SIGNAL(finished()),&pdialog,SLOT(reset()));
    connect(&watcher,SIGNAL(progressRangeChanged(int,int)),&pdialog,SLOT(setRange(int,int)));
    connect(&watcher,SIGNAL(progressValueChanged(int)),&pdialog,SLOT(setValue(int)));
    
    QFuture<void> next = QtConcurrent::map(vector,&Simulation::doTask);  // pass vector of protocols to QtConcurrent

    watcher.setFuture(next);
    pdialog.exec();
    watcher.waitForFinished();       // watch for QtConcurrent to finish

    if(watcher.isCanceled())
    {
       qDebug()<<"canceled!";
       QMessageBox::critical(this,"Cancel","Simulation canceled!");
    }
    else
    {
       qDebug()<<"finished!";
       QMessageBox::information(this,"Finish","Simulation finished!");
        Dialog graph;
        graph.exec();
    }
};

void Simulation::edit_simvars() {
    simvarMenu* menu = new simvarMenu(proto, this);
    menu->show();
    simvars_read = true;
    init_douts();
    set_sim_ready();
};

void Simulation::load_simvars() {
    simvars_read = !(bool)proto->readpars(proto->pars, proto->simvarfile);
    init_douts();
    set_sim_ready();
};

void Simulation::edit_pvars() {};

void Simulation::load_pvars() {
    pvars_read = !(bool)proto->readpvars();
    set_sim_ready();
};

void Simulation::edit_dvars() {
    dvarMenu* menu = new dvarMenu(proto, this);
    menu->show();
    dvars_read = true;
    set_sim_ready();
};

void Simulation::load_dvars() {
    dvars_read = !(bool)proto->resizemap(proto->cell->vars, proto->dvarfile, &(proto->datamap));  // use names in dvars.txt to resize datamap
    set_sim_ready();
};

void Simulation::edit_mvars() {};

void Simulation::load_mvars() {
    mvars_read = !(bool)proto->initializeMeasure(int(proto->maxmeassize));
    init_douts();
    set_sim_ready();
};

void Simulation::init_cell() {
    proto->cell = new ControlSa;
    set_cell_ready();
};
 
void Simulation::set_num_sims(int value) {
    num_sims = value;
};

QString Simulation::getDateTimeName(){
    return Dinf + "-" +Tinf;
}
