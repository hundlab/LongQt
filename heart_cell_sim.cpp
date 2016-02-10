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
#include <QGroupBox>
#include <iterator>

#include "proto.h"
#include "heart_cell_sim.h"

Simulation::Simulation(QWidget* parent){
//setup class variables
    this->parent = parent;
    proto = new Protocol();
    date_time = QDate::currentDate().toString("MMddyy");
    simvars_read = false;
    pvars_read = false;
    mvars_read = false;
    dvars_read = false;
    douts_ready = false;
    cell_ready = false;
    sim_ready = false;
    num_sims = 1;
    menu_list = new QList<tuple<QString,bool,QWidget*>>();
//local variables
    QList<std::tuple<QString,bool,QWidget*>>::iterator it;
//create layouts
    main_layout = new QGridLayout(this);
    advanced = new QGridLayout;
    file_buttons = new QHBoxLayout;
    cell_buttons = new QHBoxLayout;
//organizational widgets
    menu = new QStackedWidget();
    menu_options = new QListWidget();
    QWidget* cell_buttons_container = new QWidget();
//create buttons/combo boxes
    num_of_sims = new QSpinBox();
    num_of_sims_label = new QLabel("Number of Simulations:");
    run_button = new QPushButton("Run Simulations");
    edit_sim_button = new QPushButton("Edit Simulaiton Variables");
    load_sim_button = new QPushButton("Read Simulation Variables");
    edit_pvars_button = new QPushButton("Edit Simulation Constants");
    load_pvars_button = new QPushButton("Read Simulaiton Constants");
    edit_dvars_button = new QPushButton("Edit Output Variables");
    load_dvars_button = new QPushButton("Read Output Variables");
    edit_mvars_button = new QPushButton("Edit Measurement Variables");
    load_mvars_button = new QPushButton("Read Measurement Variables");
    load_all_button = new QPushButton("Read Variables and Constants");
    init_cell_button = new QPushButton("Use cell preference");
    cell_type = new QComboBox();
    cell_species = new QComboBox();
    next_button = new QPushButton("Next");
//add items menu_list
    menu_list->append(std::tuple<QString,bool,QWidget*> ("Cell Options",true, cell_buttons_container));
    menu_list->append(std::tuple<QString,bool,QWidget*> ("Edit Simvars",false,NULL));
    menu_list->append(std::tuple<QString,bool,QWidget*> ("Edit DVars",false, NULL));
    menu_list->append(std::tuple<QString,bool,QWidget*> ("Edit MVars",false, NULL));
    menu_list->append(std::tuple<QString,bool,QWidget*> ("Edit, PVars",false, NULL));
    menu_list->append(std::tuple<QString,bool,QWidget*> ("Run Simulation",false, run_button));
//set button/combo box inital values
    num_of_sims->setValue(num_sims);
    run_button->setEnabled(sim_ready);
    load_pvars_button->setEnabled(cell_ready);
    edit_pvars_button->setEnabled(cell_ready);
    load_dvars_button->setEnabled(cell_ready);
    edit_dvars_button->setEnabled(cell_ready);
    load_all_button->setEnabled(cell_ready);
    cell_type->addItem("Default Cell");
    cell_type->addItem("Choose Cell");
    cell_type->addItem("Ventricular");
    cell_type->addItem("Sinoatrial Node");
    cell_type->addItem("Atrial");
    cell_type->addItem("Kurata");
    cell_species->addItem("Default Species");
    cell_species->addItem("Choose Species");
    cell_species->addItem("Mouse");
    cell_species->addItem("Rabbit");
    cell_species->addItem("Human");
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
//cell_buttons
    cell_buttons->addWidget(cell_type);
    cell_buttons->addWidget(cell_species);
    cell_buttons->addWidget(init_cell_button);
    cell_buttons->addWidget(num_of_sims_label);
    cell_buttons->addWidget(num_of_sims);
    cell_buttons_container->setLayout(cell_buttons);
//menu
    for(it = menu_list->begin(); it != menu_list->end(); it++) {
        menu_options->addItem(std::get<0>(*it));
        if(std::get<1>(*it)) {
            menu->addWidget(std::get<2>(*it));
        }
    }
//main_layout
//    main_layout->addLayout(file_buttons,1,0,1,5);
//    main_layout->addLayout(advanced, max_height -1, 0,1,4);
    main_layout->addWidget(menu_options, 0,0,-1,1);
    main_layout->addWidget(menu, 0, 1);
    main_layout->addWidget(next_button, 1, 2);
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
    connect(menu_options, SIGNAL(currentRowChanged(int)), this, SLOT(list_click_aciton(int)));
    connect(next_button, SIGNAL(clicked()), this, SLOT(next_button_aciton()));
};

Simulation::~Simulation(){};

void Simulation::set_sim_ready() {
    QString name = std::get<0>(menu_list->at(5));
    QWidget* old = std::get<2>(menu_list->at(5));
    sim_ready = simvars_read && pvars_read && mvars_read && dvars_read && cell_ready && douts_ready;
    menu_list->replace(5, make_tuple(name, sim_ready, old));
    menu->insertWidget(5, std::get<2>(menu_list->at(5)));
    run_button->setEnabled(sim_ready);
};

void Simulation::set_cell_ready() {
    cell_ready = true;
    menu_list->replace(1, make_tuple(std::get<0>(menu_list->at(1)), cell_ready, new simvarMenu(proto,date_time, this)));
    menu_list->replace(2, make_tuple(std::get<0>(menu_list->at(2)), cell_ready, new dvarMenu(proto,date_time, this)));
    menu_list->replace(3, make_tuple(std::get<0>(menu_list->at(3)), cell_ready, new mvarMenu(proto,date_time, this)));
    menu_list->replace(4, make_tuple(std::get<0>(menu_list->at(4)), cell_ready, new pvarMenu(proto,date_time, this)));
    menu->insertWidget(1, std::get<2>(menu_list->at(1)));
    menu->insertWidget(2, std::get<2>(menu_list->at(2)));
    menu->insertWidget(3, std::get<2>(menu_list->at(3)));
    menu->insertWidget(4, std::get<2>(menu_list->at(4)));
    load_pvars_button->setEnabled(cell_ready);
    edit_pvars_button->setEnabled(cell_ready);
    load_dvars_button->setEnabled(cell_ready);
    edit_dvars_button->setEnabled(cell_ready);
    load_all_button->setEnabled(cell_ready);
};

void Simulation::init_douts() {
    if(simvars_read&&mvars_read) {
        proto->douts = new Output[proto->getNeededDOutputSize()];   // Array of i/o data streams
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
    QDir().mkdir("data" + date_time);
   for( i = 0; i < num_sims; i++) {
        temp = new Protocol(*proto);
        temp->readfile = "./data" + QDate::currentDate().toString("MMddyy").toStdString() + "/r"+ to_string(i) + ".dat"; // File to read SV ICs
        temp->savefile = "./data" + QDate::currentDate().toString("MMddyy").toStdString() + "/s"+ to_string(i) + ".dat"; // File to save final SV
        temp->propertyoutfile = "./data" + QDate::currentDate().toString("MMddyy").toStdString() + "/dt%d_%s" + to_string(i) + string(".dat");
        temp->dvarsoutfile = "./data" + QDate::currentDate().toString("MMddyy").toStdString() + "/dt%d_dvars"+ to_string(i) + string(".dat");
        temp->finalpropertyoutfile = "./data" + QDate::currentDate().toString("MMddyy").toStdString() + "/dss_%s"+ to_string(i) + string(".dat");
        temp->finaldvarsoutfile = "./data" + QDate::currentDate().toString("MMddyy").toStdString() + "/dss_pvars"+ to_string(i) + string(".dat");
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
    }

//       else{
//        QMessageBox msgBox;
//        msgBox.setText("Code Currently Being Modified\n Please choose a different cell.\n (Kurata)");
//        msgBox.exec();
//    }
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

void Simulation::edit_pvars() {
    pvarMenu* menu = new pvarMenu(proto, this);
    menu->show();
    pvars_read = true;
    set_sim_ready();
};

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

void Simulation::edit_mvars() {
    mvarMenu* menu = new mvarMenu(proto, this);
    menu->show();
    mvars_read = true;
    set_sim_ready();
};

void Simulation::load_mvars() {
    mvars_read = proto->readMvars(proto->measfile);
    mvars_read = mvars_read&&!(bool)proto->initializeMeasure(int(proto->maxmeassize));
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
    if(std::get<1>(menu_list->at(current_row +1))) {
        menu->setCurrentIndex(current_row +1);
        menu_options->setCurrentRow(current_row +1);
    }
}

void Simulation::leave_current(int current) {
    switch(current) {
    case 1:
        ((simvarMenu*) std::get<2>(menu_list->at(current)))->write_file();
        simvars_read = true;
        init_douts();
        set_sim_ready();
    break;
    case 2:
        ((dvarMenu*) std::get<2>(menu_list->at(current)))->write_file();
        dvars_read = true;
        set_sim_ready();
    break;
    case 3:
        ((mvarMenu*) std::get<2>(menu_list->at(current)))->write_file();
        mvars_read = true;
        init_douts();
        set_sim_ready();
    break;
    case 4:
        ((pvarMenu*) std::get<2>(menu_list->at(current)))->write_file();
        pvars_read = true;
        set_sim_ready();
    break;
    }
}
