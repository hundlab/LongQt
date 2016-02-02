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


/*########################
  begin simvarMenu class
##########################*/
simvarMenu::simvarMenu(Protocol* initial_proto, QWidget *parent)  {
//setup class variables
    proto = initial_proto;
    this->parent = parent;
    write_close = true;
//setup useful constants and aliases
    unsigned int i, row_len = 6;
    map<string,double*> pars = proto->pars;
    map<string,double*>::iterator it;
//initialize layouts and signal maps
    QGridLayout* main_layout = new QGridLayout(this);
    QGridLayout* central_layout = new QGridLayout;
    QHBoxLayout* button_group;
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
        string name = it->first;
        simvars[i]->setMaximum(std::numeric_limits<double>::max());
        button_group = new QHBoxLayout;
        button_group->addWidget(simvar_names[i]);
        button_group->addWidget(simvars[i]);
        central_layout->addLayout(button_group, (2*i)/row_len+1, (2*i)%row_len, 1, 2);
        connect(simvars[i], static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), [=] (double value) {update_pvars(pair<string,double>(name, value));});
    }
//main_layout
    main_layout->addWidget(get_vars, 0,0);
    main_layout->addWidget(set_vars, 0,1);
    main_layout->addLayout(central_layout, 1,0, i/row_len, 2*row_len); 
    main_layout->addWidget(close_button, (2*i)*row_len, 2*row_len -1);
    setLayout(main_layout); 
    setWindowTitle(tr("Simulation Variables Menu"));
//connect buttons   
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
    for(it = proto->pars.begin(), i = 0; it != proto->pars.end(); it++, i++)     {  
        simvars[i]->setValue(*(it->second));
    }
}
void simvarMenu::closeEvent(QCloseEvent* event){
    if(write_close) {
        //This is simvars
        !(bool)proto->writepars(proto->pars, string("simvars") + QDate::currentDate().toString("MMddyy").toStdString() + "-" + QTime::currentTime().toString("hm").toStdString() + string(".txt"));
    }
    event->accept();
}
bool simvarMenu::read_simvars(){

    bool ret = false;
    QString fileName = QFileDialog::getOpenFileName(this);
    if (!fileName.isEmpty()){
        proto->simvarfile = fileName.toStdString();
    ret = !(bool)proto->readpars(proto->pars, proto->simvarfile);
    }
    update_menu();
    return ret;
}
bool simvarMenu::write_simvars(){
    
    bool ret = false;
    QString fileName = QFileDialog::getOpenFileName(this);
    if (!fileName.isEmpty()){
        proto->simvarfile = fileName.toStdString();
    ret = !(bool)proto->writepars(proto->pars, proto->simvarfile);
    }
    return ret;

}
void simvarMenu::update_pvars(pair<string,double> p){
    *(proto->pars[p.first]) = p.second;
}
void simvarMenu::set_write_close(int state) {
    write_close = (bool) state;
    set_vars->setChecked(write_close);
}
/*#################################
    begin dvarMenu class
###################################*/
dvarMenu::dvarMenu(Protocol* initial_proto, QWidget *parent)  {
//setup class variables
    proto = initial_proto;
    this->parent = parent;
    write_close = true;
//setup useful constants and aliases
    unsigned int i, row_len = 6;
    map<string,double*> vars = proto->cell->vars;
    map<string,double*>::iterator it;
//initialize layouts and signal maps
    QGridLayout* main_layout = new QGridLayout(this);
    QGridLayout* central_layout = new QGridLayout;
//initialize buttons &lables
    dvars = (QCheckBox**)malloc(vars.size()*sizeof(QCheckBox*));
    get_vars = new QPushButton(tr("Import settings"), this);
    set_vars = new QCheckBox(tr("Write File on Exit"), this);
    close_button = new QPushButton(tr("Save and Exit"), this);
//    QCheckBox readflag = new QCheckBox("Read in variable files", this);
//set button inital states
    set_vars->setChecked(write_close);
//do all the work for dvars setup
    for(it = vars.begin(), i = 0; it!=vars.end(); it++,i++) {
        pair<string,double*> p (it->first, it->second);
        dvars[i] = new QCheckBox(*(new QString((it->first).c_str())), this);
        central_layout->addWidget(dvars[i], i/row_len, i%row_len);
        connect(dvars[i], &QCheckBox::stateChanged, [=] (int state) {update_datamap(p, state);});
    }
//main_layout
    main_layout->addWidget(get_vars, 0,0);
    main_layout->addWidget(set_vars, 0,1);
    main_layout->addLayout(central_layout, 1,0, i/row_len, row_len); 
    main_layout->addWidget(close_button, (2*i)*row_len, 2*row_len -1);
    setLayout(main_layout); 
    setWindowTitle(tr("Output Variables Menu"));
//connect buttons   
    connect(get_vars, SIGNAL(clicked()), this, SLOT(read_dvars())); 
    connect(set_vars, SIGNAL(stateChanged(int)), this, SLOT(set_write_close(int)));
    connect(close_button, SIGNAL(clicked()), this, SLOT(close())); 
//make menu match proto
    update_menu();
    
}
dvarMenu::~dvarMenu(){}
void dvarMenu::update_menu() {
    map<string,double*>::iterator it;
    unsigned int i; 
    for(it = proto->cell->vars.begin(), i = 0; it != proto->cell->vars.end(); it++, i++)     {  
        if(proto->datamap.count(it->first) > 0) {
            dvars[i]->setChecked(true);
        } else {
            dvars[i]->setChecked(false);
        }
    }
}
void dvarMenu::closeEvent(QCloseEvent* event){
    if(write_close) {
        !(bool)proto->writedvars(proto->datamap, string("dvars") + QDate::currentDate().toString("MMddyy").toStdString() + "-" + QTime::currentTime().toString("hm").toStdString() + string(".txt"));
    //dvars
    }
    event->accept();
}
bool dvarMenu::read_dvars(){
    bool ret = false;
    QString fileName = QFileDialog::getOpenFileName(this);
    if (!fileName.isEmpty()){
        proto->dvarfile = fileName.toStdString();
        ret = !(bool)proto->resizemap(proto->cell->vars, proto->dvarfile, &(proto->datamap));  // use names in dvars.txt to resize datamap
    }
    update_menu();
    return ret;
}
bool dvarMenu::write_dvars(){
    bool ret = false;
    QString fileName = QFileDialog::getOpenFileName(this);
    if (!fileName.isEmpty()){
        proto->dvarfile = fileName.toStdString();
    ret = !(bool)proto->writedvars(proto->datamap, proto->dvarfile);
    }
    return ret;
}
void dvarMenu::update_datamap(pair<string,double*> p, int state){
    if((state = 0)) {
        proto->datamap.erase(p.first);
    } else {
        proto->datamap.insert(p);
    }
}
void dvarMenu::set_write_close(int state) {
    write_close = (bool) state;
    set_vars->setChecked(write_close);
}
