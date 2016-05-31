#include <QMainWindow>
#include <QGridLayout>
#include <QButtonGroup>
#include <QHBoxLayout>
#include <QPushButton>
#include <QComboBox>
#include <QSpinBox>
#include <QScrollArea>
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
#include <QSplitter>

#include "protocol.h"
#include "heart_cell_sim.h"
#include "varmenu.h"
#include "dialog.h"
#include "runWidget.h"
#include "chooseProtoWidget.h"

Simulation::Simulation(QWidget* parent){
//setup class variables
    this->parent = parent;
    date_time = QDate::currentDate().toString("MMddyy") + "-" + QTime::currentTime().toString("hhmm");
//    bool was_grid = false;
//create layouts
    main_layout = new QGridLayout(this);
//organizational widgets
    menu = new QStackedWidget();
    menu_options = new QListWidget();
//create Widgets 
    QSplitter* main_splitter = new QSplitter();
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
    connect(choose, SIGNAL(protocolChanged(Protocol*)), run, SLOT(setProto(Protocol*)));
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
/*    connect(this, static_cast<void(Simulation::*)()>(&Simulation::cell_type_changed), [this,&was_grid,pvars,run] () {
        if((proto->cell->type == string("gridCell"))&&(!was_grid)) {
            int pvarsPos = menu_list.indexOf(pvars);
            menu_list.removeAt(pvarsPos);
            menu->removeWidget(pvars);
            menu_options->takeItem(pvarsPos);
            was_grid = true;
        } else if((proto->cell->type != string("gridCell"))&&was_grid) {
            int pvarsPos = menu_list.indexOf(run)-1;
            menu_list.insert(pvarsPos,pvars);
            menu->insertWidget(pvarsPos,pvars);
            menu_options->insertItem(pvarsPos,"Cell Initializers");
            was_grid = false;
        }
    });*/
    connect(run, SIGNAL(canceled()), this, SLOT(canceled()));
    connect(run, SIGNAL(finished()), this, SLOT(finished()));
    connect(run, SIGNAL(running()), this, SLOT(running()));
    connect(run, static_cast<void(runWidget::*)()>(&runWidget::running), sims ,static_cast<void(simvarMenu::*)()>(&simvarMenu::write_file));
    connect(run, static_cast<void(runWidget::*)()>(&runWidget::running), dvars ,static_cast<void(dvarMenu::*)()>(&dvarMenu::write_file));
    connect(run, static_cast<void(runWidget::*)()>(&runWidget::running), mvars ,static_cast<void(mvarMenu::*)()>(&mvarMenu::write_file));
    connect(run, static_cast<void(runWidget::*)()>(&runWidget::running), pvars ,static_cast<void(pvarMenu::*)()>(&pvarMenu::write_file));
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
        QScrollArea* scrollWraper = new QScrollArea();
        scrollWraper->setWidget(*it);
        scrollWraper->setWidgetResizable(true);
        menu->addWidget(scrollWraper);
    }
//main_splitter
    main_splitter->addWidget(menu_options);
    main_splitter->addWidget(menu);
    QList<int> tempList = QList<int>();
    tempList << 10 << 200;
    main_splitter->setSizes(tempList);
//main_layout
    main_layout->addWidget(main_splitter, 0,0,-1,1);
    main_layout->addWidget(next_button, 1, 2);
    main_layout->addWidget(cancel_button, 1, 2);
    
    this->setWindowTitle("Cell Simulation");
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
    try {
        menu_list.append(new Dialog(proto,QDir(proto->datadir.c_str()), this));
        menu->addWidget(menu_list.last());
        menu_options->addItem("Graph " + date_time);
    } catch(badFile& e) {}
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
