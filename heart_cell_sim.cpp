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
    working_dir = QStandardPaths::standardLocations(QStandardPaths::DocumentsLocation).first() + "/data" + date_time;
    menu_list = new QList<menu_object*>();
//create layouts
    main_layout = new QGridLayout(this);
//organizational widgets
    menu = new QStackedWidget();
    menu_options = new QListWidget();
//create buttons/combo boxes
    next_button = new QPushButton("Next");
    cancel_button = new QPushButton("Cancel");
//add items menu_list
    menu_list->append(new choose_proto_menu_object("Simulation type"));
    this->proto = ((chooseProtoWidget*)menu_list->last()->getWidget())->getCurrentProto();
    proto->datadir = working_dir.absolutePath().toStdString();
    connect((chooseProtoWidget*)menu_list->last()->getWidget(), SIGNAL(protocolChanged()), this, SLOT(proto_changed()));
    menu_list->append(new simvars_menu_object("Edit Simvars",proto,working_dir, this));
    connect((simvarMenu*)menu_list->last()->getWidget(), SIGNAL(cell_type_changed()), this, SLOT(cell_changed()));
    connect((simvarMenu*)menu_list->last()->getWidget(), SIGNAL(working_dir_changed(QDir)), this, SLOT(change_working_dir(QDir)));
    menu_list->append(new dvars_menu_object("Edit DVars", proto,working_dir, this));
    menu_list->append(new mvars_menu_object("Edit MVars", proto,working_dir, this));
    menu_list->append(new pvars_menu_object("Edit PVars", proto,working_dir, this));
    menu_list->append(new run_menu_object("Run Simulation", proto, working_dir));
    connect((runWidget*)menu_list->last()->getWidget(), SIGNAL(canceled()), this, SLOT(canceled()));
    connect((runWidget*)menu_list->last()->getWidget(), SIGNAL(finished()), this, SLOT(finished()));
    connect((runWidget*)menu_list->last()->getWidget(), SIGNAL(running()), this, SLOT(running()));
    connect(cancel_button, SIGNAL(clicked()),(runWidget*)menu_list->last()->getWidget(), SLOT(cancel()));
//set button/combo box inital values
    cancel_button->hide();
//menu
    for(auto it = menu_list->begin(); it != menu_list->end(); it++) {
        menu_options->addItem((*it)->name.c_str());
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
    connect(menu_options, SIGNAL(currentRowChanged(int)), this, SLOT(list_click_aciton(int)));
    connect(next_button, SIGNAL(clicked()), this, SLOT(next_button_aciton()));
};
Simulation::~Simulation(){};
void Simulation::cell_changed() {
    auto it = menu_list->begin();
    it++; it++;
    for(; it != menu_list->end(); it++) {
        int index = menu->indexOf((*it)->getWidget());
        menu->removeWidget((*it)->getWidget());
        (*it)->reset();
        menu->insertWidget(index, (*it)->getWidget());
    }
}
void Simulation::proto_changed() {
    this->proto = ((chooseProtoWidget*)menu_list->first()->getWidget())->getCurrentProto();
    proto->datadir = working_dir.absolutePath().toStdString();
    auto it = menu_list->begin();
    for(; it != menu_list->end(); it++) {
        (*it)->changeProto(proto);
    }
    it = menu_list->begin();
    it++;
    disconnect((simvarMenu*)(*it)->getWidget(), SIGNAL(cell_type_changed()), this, SLOT(cell_changed()));
    disconnect((simvarMenu*)(*it)->getWidget(), SIGNAL(working_dir_changed(QDir)), this, SLOT(change_working_dir(QDir)));
    int index = menu->indexOf((*it)->getWidget());
    menu->removeWidget((*it)->getWidget());
    (*it)->reset();
    menu->insertWidget(index, (*it)->getWidget());
    connect((simvarMenu*)(*it)->getWidget(), SIGNAL(cell_type_changed()), this, SLOT(cell_changed()));
    connect((simvarMenu*)(*it)->getWidget(), SIGNAL(working_dir_changed(QDir)), this, SLOT(change_working_dir(QDir)));

    cell_changed();
}
void Simulation::change_working_dir(QDir dir) {
    this->working_dir = dir;
    for(auto it = menu_list->begin(); it != menu_list->end(); it++) {
        (*it)->setWorkingDir(dir);
    }
};
void Simulation::list_click_aciton (int next_row) {
    menu_list->value(menu->currentIndex())->leave();
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
    qDebug()<<"finished!";
    QMessageBox::information(this,"Finish","Simulation finished!");
    menu_list->append(new graph_menu_object("Graph", proto, working_dir, this));
    menu->addWidget(menu_list->last()->getWidget());
    menu_options->addItem(menu_list->last()->name.c_str());
    date_time = QDate::currentDate().toString("MMddyy") + "-" + QTime::currentTime().toString("hhmm");
    working_dir = QStandardPaths::standardLocations(QStandardPaths::DocumentsLocation).first() + "/data" + date_time;
    change_working_dir(working_dir);
    cancel_button->hide();
    next_button->show();
}
void Simulation::running() {
    next_button->hide();
    cancel_button->show();
}
