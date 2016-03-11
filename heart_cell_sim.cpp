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

#include "protocol.h"
#include "heart_cell_sim.h"
#include "varmenu.h"
#include "dialog.h"

Simulation::Simulation(QWidget* parent){
//setup class variables
    this->parent = parent;
    proto = new Protocol();
    date_time = QDate::currentDate().toString("MMddyy") + "-" + QTime::currentTime().toString("hhmm");
    working_dir = QStandardPaths::displayName(QStandardPaths::DocumentsLocation);
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
    QStringList cell_options;
    auto cell_opitons_stl = proto->cellOptions();
    for(auto im = cell_opitons_stl.begin(); im != cell_opitons_stl.end(); im++) {
            cell_options << im->c_str();
    }
//create layouts
    main_layout = new QGridLayout(this);
    file_buttons = new QHBoxLayout;
    cell_buttons = new QHBoxLayout;
//organizational widgets
    menu = new QStackedWidget();
    menu_options = new QListWidget();
    QWidget* cell_buttons_container = new QWidget();
//create buttons/combo boxes
    run_button = new QPushButton("Run Simulations");
    load_sim_button = new QPushButton("Read Simulation Variables");
    load_pvars_button = new QPushButton("Read Simulaiton Constants");
    load_dvars_button = new QPushButton("Read Output Variables");
    load_mvars_button = new QPushButton("Read Measurement Variables");
    load_all_button = new QPushButton("Read Variables and Constants");
    init_cell_button = new QPushButton("Use cell preference");
    cell_type = new QComboBox();
    disp_working_dir = new QLineEdit();
    set_working_dir = new QPushButton("Change Save Directory");
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
    menu_list->append(std::tuple<QString,bool,QWidget*> ("Cell Options",true, cell_buttons_container));
    menu_list->append(std::tuple<QString,bool,QWidget*> ("Edit Simvars",false,NULL));
    menu_list->append(std::tuple<QString,bool,QWidget*> ("Edit DVars",false, NULL));
    menu_list->append(std::tuple<QString,bool,QWidget*> ("Edit MVars",false, NULL));
    menu_list->append(std::tuple<QString,bool,QWidget*> ("Edit PVars",false, NULL));
    menu_list->append(std::tuple<QString,bool,QWidget*> ("Run Simulation",false, run_button_container));
    menu_list->append(std::tuple<QString,bool,QWidget*> ("Graph",false, NULL));
//set button/combo box inital values
    run_button->setEnabled(sim_ready);
    load_pvars_button->setEnabled(cell_ready);
    load_dvars_button->setEnabled(cell_ready);
    load_all_button->setEnabled(cell_ready);
    cell_type->addItems(cell_options);
    disp_working_dir->setText(working_dir.dirName());
    disp_working_dir->setReadOnly(true);
    cancel_button->hide();
//add buttons to layouts
//load variables buttons
//    file_buttons->addWidget(load_sim_button);
//    file_buttons->addWidget(load_pvars_button);
//    file_buttons->addWidget(load_dvars_button);
//    file_buttons->addWidget(load_mvars_button);
    file_buttons->addWidget(load_all_button);
//cell_buttons
    cell_buttons->addWidget(cell_type);
    cell_buttons->addWidget(init_cell_button);
    cell_buttons->addWidget(disp_working_dir);
    cell_buttons->addWidget(set_working_dir);
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
    main_layout->addWidget(cancel_button, 1, 2);
    QSizePolicy sPol(QSizePolicy::Minimum, QSizePolicy::Expanding);
    menu_options->setSizePolicy(sPol);


    menu_options->setWindowTitle(date_time);
//connect buttons
    connect(run_button, SIGNAL(clicked()),this,SLOT(run_sims()));
    connect(load_sim_button, SIGNAL(clicked()),this, SLOT(load_simvars()));
    connect(load_pvars_button, SIGNAL(clicked()), this, SLOT(load_pvars()));
    connect(load_dvars_button, SIGNAL(clicked()), this, SLOT(load_dvars()));
    connect(load_mvars_button, SIGNAL(clicked()), this, SLOT(load_mvars()));
    connect(load_all_button, SIGNAL(clicked()), this, SLOT(load_simvars()));
    connect(load_all_button, SIGNAL(clicked()), this, SLOT(load_pvars()));
    connect(load_all_button, SIGNAL(clicked()), this, SLOT(load_dvars()));
    connect(load_all_button, SIGNAL(clicked()), this, SLOT(load_mvars()));
    connect(init_cell_button, SIGNAL(clicked()), this, SLOT(init_cell()));
    connect(menu_options, SIGNAL(currentRowChanged(int)), this, SLOT(list_click_aciton(int)));
    connect(next_button, SIGNAL(clicked()), this, SLOT(next_button_aciton()));
    connect(set_working_dir, SIGNAL(clicked()), this, SLOT(set_working_dir_action()));
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
    working_dir.mkdir("data" + date_time);
    menu_list->replace(1, make_tuple(std::get<0>(menu_list->at(1)), cell_ready, new simvarMenu(proto,working_dir.absolutePath() + "/data" + date_time, this)));
    menu_list->replace(2, make_tuple(std::get<0>(menu_list->at(2)), cell_ready, new dvarMenu(proto,working_dir.absolutePath() + "/data" + date_time, this)));
    menu_list->replace(3, make_tuple(std::get<0>(menu_list->at(3)), cell_ready, new mvarMenu(proto,working_dir.absolutePath() + "/data" + date_time, this)));
    menu_list->replace(4, make_tuple(std::get<0>(menu_list->at(4)), cell_ready, new pvarMenu(proto,working_dir.absolutePath() + "/data" + date_time, this)));
    menu->insertWidget(1, std::get<2>(menu_list->at(1)));
    menu->insertWidget(2, std::get<2>(menu_list->at(2)));
    menu->insertWidget(3, std::get<2>(menu_list->at(3)));
    menu->insertWidget(4, std::get<2>(menu_list->at(4)));
    load_pvars_button->setEnabled(cell_ready);
    load_dvars_button->setEnabled(cell_ready);
    load_all_button->setEnabled(cell_ready);
};
void Simulation::init_douts() {
    if(simvars_read&&mvars_read) {
       proto->douts = new Output[proto->getNeededDOutputSize()];   // Array of i/o data streams
       douts_ready = true;
    }
};
void Simulation::doTask(Protocol& toRun) {
//    toRun.runSim(); 
    toRun.runTrial();
};
void Simulation::run_sims() {
    unsigned int i = 0;
    Protocol* temp;
    next_button->hide();
    cancel_button->show();
    run_button->setEnabled(false);

    for( i = 0; i < proto->numtrials; i++) {
        proto->setTrial(i);
        temp = new Protocol(*proto);
        temp->readfile = working_dir.absolutePath().toStdString() + "/data" + date_time.toStdString() + "/r"+ to_string(i) + ".dat"; // File to read SV ICs
        temp->savefile = working_dir.absolutePath().toStdString() + "/data" + date_time.toStdString() + "/s"+ to_string(i) + ".dat"; // File to save final SV
        temp->propertyoutfile = working_dir.absolutePath().toStdString() + "/data" + date_time.toStdString() + "/dt%d_%s" + string(".dat");
        temp->dvarsoutfile = working_dir.absolutePath().toStdString() + "/data" + date_time.toStdString() + "/dt%d_dvars" + string(".dat");
        temp->finalpropertyoutfile = working_dir.absolutePath().toStdString() + "/data" + date_time.toStdString() + "/dss%d_%s" + string(".dat");
        temp->finaldvarsoutfile = working_dir.absolutePath().toStdString() + "/data" + date_time.toStdString() + "/dss%d_pvars" + string(".dat");
        vector.append(*temp);
    } 

 
    connect(cancel_button,SIGNAL(clicked()),&watcher,SLOT(cancel()));  // connect signals and slots for watcher and dialog
    connect(cancel_button,SIGNAL(clicked()),this,SLOT(canceled()));
    connect(&watcher,SIGNAL(finished()),this,SLOT(finished()));
    connect(&watcher,SIGNAL(finished()),pdialog,SLOT(reset()));
    connect(&watcher,SIGNAL(progressRangeChanged(int,int)),pdialog,SLOT(setRange(int,int)));
    connect(&watcher,SIGNAL(progressValueChanged(int)),pdialog,SLOT(setValue(int)));
    
    QFuture<void> next = QtConcurrent::map(vector,&Simulation::doTask);  // pass vector of protocols to QtConcurrent

    watcher.setFuture(next);

};
void Simulation::load_simvars() {
    simvars_read = !(bool)proto->readpars(proto->pars, proto->simvarfile);
    init_douts();
    set_sim_ready();
};
void Simulation::load_pvars() {
    pvars_read = !(bool)proto->readpvars();
    set_sim_ready();
};
void Simulation::load_dvars() {
    dvars_read = !(bool)proto->resizemap(proto->cell->vars, proto->dvarfile, &(proto->datamap));  // use names in dvars.txt to resize datamap
    set_sim_ready();
};
void Simulation::load_mvars() {
    mvars_read = true;//mvars_read&&!(bool)proto->initializeMeasure(int(proto->maxmeassize));
    init_douts();
    set_sim_ready();
};
void Simulation::init_cell() {
    if(proto->setCell(cell_type->currentText().toStdString())) {
        set_cell_ready();
    }
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
    if(std::get<1>(menu_list->value(current_row +1))) {
        menu->setCurrentIndex(current_row +1);
        menu_options->setCurrentRow(current_row +1);
    }
}
void Simulation::set_working_dir_action() {
    QString path = QFileDialog::getExistingDirectory();
    if(!path.isEmpty()) {
        working_dir = path;
    }
    disp_working_dir->setText(working_dir.dirName());
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
    menu_list->replace(6, make_tuple(std::get<0>(menu_list->at(6)), true, new Dialog(proto, working_dir.absolutePath()+"/data" + date_time, this)));
    menu->insertWidget(6, std::get<2>(menu_list->at(6)));
    date_time = QDate::currentDate().toString("MMddyy") + "-" + QTime::currentTime().toString("hhmm");
    cancel_button->hide();
    next_button->show();
    run_button->setEnabled(false);
}
