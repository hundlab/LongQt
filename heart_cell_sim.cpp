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
#include "runWidget.h"
#include "chooseProtoWidget.h"
#include "dvars_menu_object.cpp"
#include "pvars_menu_object.cpp"
#include "simvars_menu_object.cpp"
#include "mvars_menu_object.cpp"
#include "graph_menu_object.cpp"
#include "run_menu_object.cpp"
#include "choose_proto_menu_object.cpp"

Simulation::Simulation(QWidget* parent){
//setup class variables
    this->parent = parent;
    date_time = QDate::currentDate().toString("MMddyy") + "-" + QTime::currentTime().toString("hhmm");
//create layouts
    main_layout = new QGridLayout(this);
//organizational widgets
    menu = new QStackedWidget();
    menu_options = new QListWidget();
//create Widgets 
    next_button = new QPushButton("Next");
    cancel_button = new QPushButton("Cancel");
    chooseProtoWidget* choose = new chooseProtoWidget(this);
    this->proto = choose->getCurrentProto();
    proto->datadir = (QStandardPaths::standardLocations(QStandardPaths::DocumentsLocation).first() + "/data" + date_time).toStdString();
    simvarMenu* sims = new simvarMenu(proto,QDir(proto->datadir.c_str()), this);
    dvarMenu* dvars = new dvarMenu(proto,QDir(proto->datadir.c_str()) , this);
    mvarMenu* mvars =  new mvarMenu(proto,QDir(proto->datadir.c_str()), this);
    pvarMenu* pvars =  new pvarMenu(proto,QDir(proto->datadir.c_str()), this);
    runWidget* run = new runWidget(proto,QDir(proto->datadir.c_str()));
//add items menu_list
    menu_list.append(choose);
    menu_list.append(sims);
    menu_list.append(dvars);
    menu_list.append(mvars);
    menu_list.append(pvars);
    menu_list.append(run);

    connect(choose, SIGNAL(protocolChanged(Protocol*)), sims, SLOT(changeProto(Protocol*)));
    connect(choose, SIGNAL(protocolChanged(Protocol*)), dvars, SLOT(changeProto(Protocol*)));
    connect(choose, SIGNAL(protocolChanged(Protocol*)), pvars, SLOT(changeProto(Protocol*)));
    connect(choose, SIGNAL(protocolChanged(Protocol*)), mvars, SLOT(changeProto(Protocol*)));
    connect(choose, SIGNAL(protocolChanged(Protocol*)), this, SLOT(changeProto(Protocol*)));
    connect(choose, SIGNAL(cell_type_changed()), this, SIGNAL(cell_type_changed()));
    connect(this, SIGNAL(cell_type_changed()), choose, SLOT(cellChangedSlot()));
    connect(sims, SIGNAL(working_dir_changed(QDir&)), this, SIGNAL(working_dir_changed(QDir&)));
    connect(sims, SIGNAL(cell_type_changed()), this, SIGNAL(cell_type_changed()));
    connect(this, SIGNAL(working_dir_changed(QDir&)), dvars, SLOT(setWorkingDir(QDir&)));
    connect(this, SIGNAL(working_dir_changed(QDir&)), mvars, SLOT(setWorkingDir(QDir&)));
    connect(this, SIGNAL(working_dir_changed(QDir&)), pvars, SLOT(setWorkingDir(QDir&)));
    connect(this, SIGNAL(working_dir_changed(QDir&)), run, SLOT(setWorkingDir(QDir&)));
    connect(this, SIGNAL(cell_type_changed()), sims, SLOT(changeCellType()));
    connect(this, SIGNAL(cell_type_changed()), dvars, SLOT(reset()));
    connect(this, SIGNAL(cell_type_changed()), mvars, SLOT(reset()));
    connect(this, SIGNAL(cell_type_changed()), pvars, SLOT(reset()));
    connect(run, SIGNAL(canceled()), this, SLOT(canceled()));
    connect(run, SIGNAL(finished()), this, SLOT(finished()));
    connect(run, SIGNAL(running()), this, SLOT(running()));
    connect(cancel_button, SIGNAL(clicked()),run, SLOT(cancel()));
//set button/combo box inital values
    cancel_button->hide();
//menu
    menu_options->addItem("Simulation Protocol");
    menu_options->addItem("Simulation Varibales");
    menu_options->addItem("Tracked Cell Values");
    menu_options->addItem("Measured Cell Values");
    menu_options->addItem("Cell Initializers");
    menu_options->addItem("Run Simulation");
    for(auto it = menu_list.begin(); it != menu_list.end(); it++) {
        menu->addWidget(*it);
    }
//main_layout
    main_layout->addWidget(menu_options, 0,0,-1,1);
    main_layout->addWidget(menu, 0, 1);
    main_layout->addWidget(next_button, 1, 2);
    main_layout->addWidget(cancel_button, 1, 2);
    QSizePolicy sPol(QSizePolicy::Minimum, QSizePolicy::Expanding);
    menu_options->setSizePolicy(sPol);
    menu_options->setWindowTitle(date_time);
    showMaximized();
//connect buttons
    connect(menu_options, SIGNAL(currentRowChanged(int)), this, SLOT(list_click_aciton(int)));
    connect(next_button, SIGNAL(clicked()), this, SLOT(next_button_aciton()));
};
Simulation::~Simulation(){};
void Simulation::changeProto(Protocol* proto) {
    this->proto = proto;
}
void Simulation::list_click_aciton (int next_row) {
    menu->setCurrentIndex(next_row);
    menu_options->setCurrentRow(next_row);
}
void Simulation::next_button_aciton () {
    int current_row = menu->currentIndex();
    if(menu->count() > current_row +1) {
        menu->setCurrentIndex(current_row +1);
        menu_options->setCurrentRow(current_row +1);
    }
}
void Simulation::canceled() {
    qDebug()<<"canceled!";
    QMessageBox::critical(this,"Cancel","Simulation canceled!");
    cancel_button->hide();
    next_button->show();
}
void Simulation::finished() {
    menu_list.append(new Dialog(proto,QDir(proto->datadir.c_str()), this));
    menu->addWidget(menu_list.last());
    menu_options->addItem("Graph " + date_time);
    date_time = QDate::currentDate().toString("MMddyy") + "-" + QTime::currentTime().toString("hhmm");
    QDir working_dir = (QStandardPaths::standardLocations(QStandardPaths::DocumentsLocation).first() + "/data" + date_time); 
    proto->datadir = working_dir.absolutePath().toStdString();
    emit working_dir_changed(working_dir);
    cancel_button->hide();
    next_button->show();
}
void Simulation::running() {
    next_button->hide();
    cancel_button->show();
}
