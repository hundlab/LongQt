#include <QWidget>
#include <QDebug>
#include <QMessageBox>
#include <QDoubleSpinBox>
#include <QLabel>
#include <QGridLayout>
#include <QString>
#include <QPushButton>
#include <QFile>
#include <QFileDialog>
#include <QCheckBox>
#include <QDate>
#include <QCloseEvent>
#include <limits>
#include <QHBoxLayout>

#include "varmenu.h"
#include "proto.h"



simvarMenu::simvarMenu(Protocol* initial_proto, QWidget *parent)  {
//setup class variables
    proto = *initial_proto;
    this->parent = parent;
    write_close = true;
//setup useful constants and aliases
    unsigned int i, row_len = 6;
    map<string,double*> pars = proto.pars;
    map<string,double*>::iterator it;
//initialize layouts and signal maps
    QGridLayout* main_layout = new QGridLayout(this);
    QGridLayout* central_layout = new QGridLayout;
    QHBoxLayout* button_group;
    simvar_map = new QSignalMapper(this);
//initialize buttons &lables
    simvars = (QDoubleSpinBox**)malloc(pars.size()*sizeof(QDoubleSpinBox*));
    simvar_names = (QLabel**)malloc(pars.size()*sizeof(QLabel*));
    get_vars = new QPushButton(tr("Import settings"), this);
    set_vars = new QCheckBox(tr("Write File on Exit"), this);
    close_button = new QPushButton("Save and Exit", this);
//    QCheckBox readflag = new QCheckBox("Read in variable files", this);
//set button inital states
    set_vars->setChecked(write_close);
//do all the work for simvars setup
    for(it = pars.begin(), i = 0; it!=pars.end(); it++,i++) {
        simvars[i] = new QDoubleSpinBox(this);
        simvar_names[i] = new QLabel(*(new QString((it->first).c_str())),this);
        simvars[i]->setMaximum(std::numeric_limits<double>::max());
        button_group = new QHBoxLayout;
        button_group->addWidget(simvar_names[i]);
        button_group->addWidget(simvars[i]);
        central_layout->addLayout(button_group, (2*i)/row_len+1, (2*i)%row_len, 1, 2);
//        main_layout->addWidget(simvar_names[i],(2*i)/row_len+1, (2*i)%row_len);
//        main_layout->addWidget(simvars[i],(2*i+1)/row_len+1, (2*i+1)%row_len);
        connect(simvars[i], SIGNAL(editingFinished()),simvar_map, SLOT(map()));
        simvar_map->setMapping(simvars[i],i);
    }
//main_layout
    main_layout->addWidget(get_vars, 0,0);
    main_layout->addWidget(set_vars, 0,1);
    main_layout->addLayout(central_layout, 1,0, i/row_len, 2*row_len); 
    main_layout->addWidget(close_button, (2*i)*row_len, 2*row_len -1);
    setLayout(main_layout); 
    setWindowTitle(tr("Simulation Variables Menu"));
//connect buttons   
    connect(simvar_map, SIGNAL(mapped(int)), this, SLOT(update_pvars(int)));
    connect(get_vars, SIGNAL(clicked()), this, SLOT(read_simvars())); 
    connect(set_vars, SIGNAL(stateChanged(int)), this, SLOT(set_write_close(int)));
    connect(close_button, SIGNAL(clicked()), this, SLOT(close())); 
//make menu match proto
    update_menu();
    
}

simvarMenu::~simvarMenu(){}

void simvarMenu::update_menu() {
    map<string,double*>::iterator it;
    unsigned int i; 
    for(it = proto.pars.begin(), i = 0; it != proto.pars.end(); it++, i++)     {  
        simvars[i]->setValue(*(it->second));
    }
}

void simvarMenu::closeEvent(QCloseEvent* event){
    if(write_close) {
        !(bool)proto.writepars(proto.pars, string("simvars") + QDate::currentDate().toString("MMddyy").toStdString() + string(".txt"));
    }
    event->accept();
}

bool simvarMenu::read_simvars(){

    bool ret = false;
    QString fileName = QFileDialog::getOpenFileName(this);
    if (!fileName.isEmpty()){
        proto.simvarfile = fileName.toStdString();
    ret = !(bool)proto.readpars(proto.pars, proto.simvarfile);
    }
    update_menu();
    return ret;
}

bool simvarMenu::write_simvars(){
    
    bool ret = false;
    QString fileName = QFileDialog::getOpenFileName(this);
    if (!fileName.isEmpty()){
        proto.simvarfile = fileName.toStdString();
    ret = !(bool)proto.writepars(proto.pars, proto.simvarfile);
    }
    return ret;

}

void simvarMenu::update_pvars(int id){
    *(proto.pars[simvar_names[id]->text().toStdString()]) = simvars[id]->value();
}

void simvarMenu::set_write_close(int state) {
    write_close = (bool) state;
    set_vars->setChecked(write_close);
}
