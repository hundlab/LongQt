#include <QMainWindow>
#include <QGridLayout>
#include <QButtonGroup>
#include <QHBoxLayout>
#include <QPushButton>
#include <QComboBox>
#include <QSpinBox>

#include "proto.h"
#include "heart_cell_sim.h"
#include "varmenu.h"

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
//local variables
    unsigned int max_height = 8;
    unsigned int max_width = 14;
//create layouts
    QGridLayout* main_layout = new QGridLayout(this);
    QButtonGroup* advanced_buttons = new QButtonGroup(this);
    QGridLayout* advanced = new QGridLayout();
    QHBoxLayout* file_buttons = new QHBoxLayout();
//create buttons/combo boxes
    num_of_sims = new QSpinBox(this);
    run_button = new QPushButton("Run Simulations", this);
    edit_sim_button = new QPushButton("Edit Simulaiton Variables", this);
    load_sim_button = new QPushButton("Read Simulation Variables", this);
    edit_pvars_button = new QPushButton("Edit Simulation Constants", this);
    load_pvars_button = new QPushButton("Read Simulaiton Constants", this);
    edit_dvars_button = new QPushButton("Edit Output Variables", this);
    load_dvars_button = new QPushButton("Read Output Variables", this);
    edit_mvars_button = new QPushButton("Edit Measurement Variables", this);
    load_mvars_button = new QPushButton("Read Measurement Variables", this);
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
    advanced_buttons->addButton(edit_sim_button);
    advanced->addWidget(edit_sim_button, 0,0);
    advanced->addWidget(edit_pvars_button,0,1);
    advanced->addWidget(edit_dvars_button,0,2);
    advanced->addWidget(edit_mvars_button,0,3);
//load variables buttons
    file_buttons->addWidget(load_sim_button);
    file_buttons->addWidget(load_pvars_button);
    file_buttons->addWidget(load_dvars_button);
    file_buttons->addWidget(load_mvars_button);
//main_layout
    main_layout->addWidget(cell_type, 0,0,1,1);
    main_layout->addWidget(cell_species, 0,1,1,1);
    main_layout->addWidget(num_of_sims,0,2,1,1);
    main_layout->addWidget(init_cell_button,0,3,1,1);
    main_layout->addLayout(file_buttons,1,0,1,4);
    main_layout->addLayout(advanced, max_height -1, 0,1,4);
    main_layout->addWidget(run_button, max_height, max_width);
//connect buttons
    connect(run_button, SIGNAL(clicked()),this,SLOT(run_sims()));
    connect(edit_sim_button, SIGNAL(clicked()), this, SLOT(edit_simvars()));
    connect(load_sim_button, SIGNAL(clicked()),this, SLOT(load_simvars()));
    connect(edit_pvars_button, SIGNAL(clicked()), this, SLOT(edit_pvars()));
    connect(load_pvars_button, SIGNAL(clicked()), this, SLOT(load_pvars()));
    connect(edit_dvars_button, SIGNAL(clicked()), this, SLOT(edit_pvars()));
    connect(load_dvars_button, SIGNAL(clicked()), this, SLOT(load_dvars()));
    connect(edit_mvars_button, SIGNAL(clicked()), this, SLOT(edit_mvars()));
    connect(load_mvars_button, SIGNAL(clicked()), this, SLOT(load_mvars()));
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
};

void Simulation::init_douts() {
    if(simvars_read&&mvars_read) {
        proto->douts = new Output[proto->getNeededDOutputSize()];   // Array of i/o data streams
    douts_ready = true;
    }
};

void Simulation::run_sims() {
};

void Simulation::edit_simvars() {
    simvarMenu* menu = new simvarMenu(proto, this);
    menu->show();
    simvars_read = true;
    set_sim_ready();
};

void Simulation::load_simvars() {
    simvars_read = !(bool)proto->readpars(proto->pars, proto->simvarfile);
    set_sim_ready();
};

void Simulation::edit_pvars() {};

void Simulation::load_pvars() {
    pvars_read = !(bool)proto->readpvars();
    set_sim_ready();
};

void Simulation::edit_dvars() {};

void Simulation::load_dvars() {
    dvars_read = !(bool)proto->resizemap(proto->cell->vars, proto->dvarfile, &(proto->datamap));  // use names in dvars.txt to resize datamap
    set_sim_ready();
};

void Simulation::edit_mvars() {};

void Simulation::load_mvars() {
    mvars_read = !(bool)proto->initializeMeasure(int(proto->maxmeassize));
    set_sim_ready();
};

void Simulation::init_cell() {
    proto->cell = new ControlSa;
    set_cell_ready();
};
 
void Simulation::set_num_sims(int value) {
    num_sims = value;
};
