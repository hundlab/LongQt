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
#include <QSettings>

#include "protocol.h"
#include "heart_cell_sim.h"
#include "varmenu.h"
#include "graph.h"
#include "runWidget.h"
#include "chooseProtoWidget.h"

Simulation::Simulation(int protoType, QString simvarFile,QString dvarFile,QString measFile,QString pvarFile, QWidget* parent){
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
    about_button = new QPushButton(QIcon::fromTheme("help-about"),"About");
    next_button = new QPushButton("Next");
    cancel_button = new QPushButton("Cancel");
    chooseProtoWidget* choose = new chooseProtoWidget(this);
    if(0 <= protoType  && protoType <= 2) {
        choose->changeProto(protoType);
    }
    this->proto = choose->getCurrentProto();
    if(simvarFile != "") {
        proto->readpars(simvarFile.toStdString());
    }
    if(dvarFile != "") {
        proto->readdvars(dvarFile.toStdString());
    }
    if(measFile != "") {
        proto->readMvarsFile(measFile.toStdString());
    }
    if(pvarFile != "") {
        proto->parsemixedmap(proto->cell->pars, measFile.toStdString() ,&proto->pnames, &proto->pvals);
    }
    proto->datadir = (QStandardPaths::standardLocations(QStandardPaths::DocumentsLocation).first() + "/data" + date_time).toStdString();
    proto->cellStateDir = proto->datadir;
    simvarMenu* sims = new simvarMenu(proto,QDir(proto->datadir.c_str()), this);
    dvarMenu* dvars = new dvarMenu(proto,QDir(proto->datadir.c_str()) , this);
    mvarMenu* mvars =  new mvarMenu(proto,QDir(proto->datadir.c_str()), this);
    pvarMenu* pvars =  new pvarMenu(proto,QDir(proto->datadir.c_str()), this);
    runWidget* run = new runWidget(proto,QDir(proto->datadir.c_str()));
//add items menu_list
    menu_list.append(choose);
    menu_list.append(sims);
    menu_list.append(pvars);
    menu_list.append(dvars);
    menu_list.append(mvars);
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
    connect(this, SIGNAL(working_dir_changed(QDir&)), sims, SLOT(setWorkingDir(QDir&)));
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
//    connect(run, SIGNAL(finished()), choose, SLOT(resetProto()));
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
    menu_options->addItem("Set Protocol");
    menu_options->item(menu_options->count()-1)->setToolTip("");
    menu_options->addItem("Set Sim. Parameters");
    menu_options->item(menu_options->count()-1)->setToolTip("Change model parameters (e.g. change an ion channel conductance)");
    menu_options->addItem("Set Model Parameters");
    menu_options->item(menu_options->count()-1)->setToolTip("");
    menu_options->addItem("Select Output");
    menu_options->item(menu_options->count()-1)->setToolTip("");
    menu_options->addItem("Select Measure Props.");
    menu_options->item(menu_options->count()-1)->setToolTip("Measure properties related to output variables (e.g. action potential duration, calcium amplitude)");
    menu_options->addItem("Run Simulation");
    menu_options->item(menu_options->count()-1)->setToolTip("");
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
    main_layout->addWidget(about_button, 0, 2);
    main_layout->addWidget(next_button, 2, 2);
    main_layout->addWidget(cancel_button, 2, 2);
    
    this->setWindowTitle("LongQt");
    this->showMaximized();
//connect buttons
    connect(about_button, &QPushButton::clicked, [this] () {
        if(QMessageBox::RestoreDefaults == QMessageBox::information(NULL,tr("LongQt Information"),tr("Copyright© 2016 Thomas J. Hund\nLicense: Revised BSD License\nEmail: thomas.hund@osumc.edu\nVersion 0.1\nContributers:\n\tThomas Hund\n\tBirce Ela Onal\n\tDanielle Beckley\n\tDaniel Gratz\n"),QMessageBox::Close|QMessageBox::RestoreDefaults,QMessageBox::Close)) {
            QSettings settings;
            settings.setValue("showHelp",true);
        }
    });
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
    QMessageBox::information(0,"Folder", "Simulation is finished!\n The folder is named: " + QString(proto->datadir.c_str()));
    try {
        menu_list.append(new Dialog(QDir(proto->datadir.c_str()), this));        
        menu->addWidget(menu_list.last());
        menu_options->addItem("Graph " + QFileInfo(proto->datadir.c_str()).baseName());
    } catch(badFile& e) {
        cerr << e.what() << ": " << "data files not readable" << endl;
    }
    date_time = QDate::currentDate().toString("MMddyy") + "-" + QTime::currentTime().toString("hhmm");
    QDir working_dir = (QStandardPaths::standardLocations(QStandardPaths::DocumentsLocation).first() + "/data" + date_time);
    for(int i = 1; working_dir.exists(); i++) {
        working_dir = (QStandardPaths::standardLocations(QStandardPaths::DocumentsLocation).first() + "/data" + date_time + "_" + QString::number(i));
    }
    proto->datadir = working_dir.absolutePath().toStdString();
    proto->cellStateDir = proto->datadir;
    emit working_dir_changed(working_dir);
    cancel_button->hide();
    next_button->show();
}
void Simulation::running() {
    next_button->hide();
    cancel_button->show();
}
