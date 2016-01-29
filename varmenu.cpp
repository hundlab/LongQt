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
#include <QListWidget>
#include <QComboBox>
#include <QTableWidget>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QHeaderView>

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
        !(bool)proto->writepars(proto->pars, string("simvars") + QDate::currentDate().toString("MMddyy").toStdString() + string(".txt"));
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
        !(bool)proto->writedvars(proto->datamap, string("dvars") + QDate::currentDate().toString("MMddyy").toStdString() + string(".txt"));
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

/*#################################
    begin mvarMenu class
###################################*/
mvarMenu::mvarMenu(Protocol* initial_proto, QWidget *parent)  {
//setup class variables
    proto = initial_proto;
    this->parent = parent;
    write_close = true;
//setup useful constants and aliases
    unsigned int row_len = 6;
    std::map<string,double*>::iterator it;
    Measure temp = Measure();
//initialize layouts and signal maps
    QGridLayout* main_layout = new QGridLayout(this);
    QGridLayout* central_layout = new QGridLayout;
//initialize buttons &lables
    get_vars = new QPushButton(tr("Import settings"), this);
    set_vars = new QCheckBox(tr("Write File on Exit"), this);
    close_button = new QPushButton(tr("Save and Exit"), this);
    vars_view = new QListWidget(this);
    meas_view = new QListWidget(this);
    meas_list_label = new QLabel("Variables to Track",this);
    vars_list_label = new QLabel("Aspects to Measure", this);
    addto_vars_options = new QComboBox(this);
    addto_meas_options = new QComboBox(this);
    addto_meas_list_button = new QPushButton("+", this);
    removefr_meas_list_button = new QPushButton("-", this);
    addto_vars_list_button = new QPushButton("+", this);
    removefr_vars_list_button = new QPushButton("-", this);
//    QCheckBox readflag = new QCheckBox("Read in variable files", this);
//set button inital states
    set_vars->setChecked(write_close);
    for(it = proto->cell->vars.begin(); it != proto->cell->vars.end(); it++) {
        addto_vars_options->addItem(it->first.c_str());
    }
    for(it = temp.varmap.begin(); it != temp.varmap.end(); it++) {
        addto_meas_options->addItem(it->first.c_str());
    }
//central_layout
    central_layout->addWidget(vars_list_label, 0,0);
    central_layout->addWidget(meas_list_label, 0,2);
    central_layout->addWidget(vars_view, 1,0,1,2);
    central_layout->addWidget(meas_view, 1,2,1,2);
    central_layout->addWidget(addto_vars_options, 2,0,1,2);
    central_layout->addWidget(addto_meas_options, 2,2,1,2);
    central_layout->addWidget(addto_vars_list_button,3,0);
    central_layout->addWidget(removefr_vars_list_button,3,1);
    central_layout->addWidget(addto_meas_list_button,3,2);
    central_layout->addWidget(removefr_meas_list_button,3,3);
//main_layout
    main_layout->addWidget(get_vars, 0,0);
    main_layout->addWidget(set_vars, 0,1);
    main_layout->addLayout(central_layout, 1,0, row_len, row_len); 
    main_layout->addWidget(close_button, row_len +1, row_len -1);
    setLayout(main_layout); 
    setWindowTitle(tr("Output Variables Menu"));
//connect buttons   
    connect(get_vars, SIGNAL(clicked()), this, SLOT(read_mvars())); 
    connect(vars_view, &QListWidget::currentRowChanged, this, &mvarMenu::switch_var);
    connect(addto_vars_list_button, &QPushButton::clicked, this, &mvarMenu::addto_vars_list);
    connect(removefr_vars_list_button, &QPushButton::clicked, this, &mvarMenu::removefr_vars_list);
    connect(addto_meas_list_button, &QPushButton::clicked, this, &mvarMenu::addto_meas_list);
    connect(removefr_meas_list_button, &QPushButton::clicked, this, &mvarMenu::removefr_meas_list);
    connect(set_vars, SIGNAL(stateChanged(int)), this, SLOT(set_write_close(int)));
    connect(close_button, SIGNAL(clicked()), this, SLOT(close())); 
//make menu match proto
    update_menu(-1);
}

mvarMenu::~mvarMenu(){}

void mvarMenu::update_menu(int row) {
    unsigned int i,j;

    meas_view->clear();

    for(i = 0; i < proto->mvnames.size(); i++) {
        QList<QListWidgetItem*> vars_item = vars_view->findItems(proto->mvnames[i].c_str(),Qt::MatchExactly);
        if(vars_item.empty()) {
            vars_view->addItem(proto->mvnames[i].c_str());
        }
    }
    vars_view->setCurrentRow(row);
    if(vars_view->currentRow() >= 0) {
        for(j = 0; j < proto->mpnames[vars_view->currentRow()].size(); j++){
            meas_view->addItem(proto->mpnames[vars_view->currentRow()][j].c_str());
        }
    }
    
    
}

void mvarMenu::closeEvent(QCloseEvent* event){
    proto->initializeMeasure(proto->maxmeassize);
    if(write_close) {
        !(bool)proto->write2Dmap( proto->mvnames, proto->mpnames, string("mvars") + QDate::currentDate().toString("MMddyy").toStdString() + string(".txt"));
    }
    event->accept();
}

bool mvarMenu::read_mvars(){

    bool ret = false;
    QString fileName = QFileDialog::getOpenFileName(this);
    if (!fileName.isEmpty()){
        proto->measfile = fileName.toStdString();
        ret = !(bool)proto->parse2Dmap(proto->cell->vars,Measure().varmap, proto->measfile, &proto->mvnames, &proto->mpnames);
//        ret = !(bool)proto->initializeMeasure(int(proto->maxmeassize));//create measure from mvarfile
   }
    update_menu(-1);
    return ret;
}

bool mvarMenu::write_mvars(){
    
    bool ret = false;
    QString fileName = QFileDialog::getOpenFileName(this);
    if (!fileName.isEmpty()){
        proto->measfile = fileName.toStdString();
        ret = !(bool)proto->write2Dmap(proto->mvnames, proto->mpnames, proto->measfile);
    }
    return ret;

}

void mvarMenu::set_write_close(int state) {
    write_close = (bool) state;
    set_vars->setChecked(write_close);
}

void mvarMenu::addto_meas_list(){
    int var_row = vars_view->currentRow();
    QString to_add = addto_meas_options->currentText();

    if(meas_view->findItems(to_add, Qt::MatchExactly).empty()) {
        proto->mpnames[var_row].push_back(to_add.toStdString());
    }
    update_menu(var_row);
};

void mvarMenu::removefr_meas_list(){
    int meas_row = meas_view->currentRow();
    int var_row = vars_view->currentRow();

    if(meas_view->takeItem(meas_row) == 0) {
        return;
    }
    if(meas_view->count() == 0) {
        removefr_vars_list();
        return;
    }
    meas_view->setCurrentRow(meas_row -1);
    proto->mpnames[var_row].erase(proto->mpnames[var_row].begin() + meas_row);
    update_menu(vars_view->currentRow());
};

void mvarMenu::addto_vars_list(){
    QString to_add = addto_vars_options->currentText();

    if(vars_view->findItems(to_add, Qt::MatchExactly).empty()) {
        proto->mvnames.push_back(to_add.toStdString());
        proto->mpnames.push_back(vector<string>());
        update_menu(proto->mpnames.size() -1);
        addto_meas_list();
    }
//    update_menu(proto->mpnames.size() -1);
};

void mvarMenu::removefr_vars_list(){
    int var_row = vars_view->currentRow();
    if(vars_view->takeItem(var_row) == 0) {
        return;
    }
    proto->mvnames.erase(proto->mvnames.begin() + var_row);
    proto->mpnames.erase(proto->mpnames.begin() + var_row);
    update_menu(var_row -1);
};

void mvarMenu::switch_var(int row){
    update_menu(row);
};

/*#################################
    begin pvarMenu class
###################################*/
pvarMenu::pvarMenu(Protocol* initial_proto, QWidget *parent)  {
//setup class variables
    proto = initial_proto;
    this->parent = parent;
    write_close = true;
    pvals_options = new QStringList[3]();
    pvals_options[0] << "random" << "iter" << "#";
    pvals_options[1] << "lognormal" << "normal";
    pvals_options[2] << "logdistribution" << "#";
//setup useful constants and aliases
    unsigned int row_len = 5;
    std::map<string,double*>::iterator it;
//initialize layouts and signal maps
    QGridLayout* main_layout = new QGridLayout(this);
    central_layout = new QGridLayout;
//initialize buttons &lables
    get_vars = new QPushButton(tr("Import settings"), this);
    set_vars = new QCheckBox(tr("Write File on Exit"), this);
    pvar_table = new QTableWidget(0,5);
    close_button = new QPushButton(tr("Save and Exit"), this);
//    QCheckBox readflag = new QCheckBox("Read in variable files", this);
//set button inital states
    set_vars->setChecked(write_close);
    pvar_table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    pvar_table->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
//central_layout
//main_layout
    main_layout->addWidget(get_vars, 0,0);
    main_layout->addWidget(set_vars, 0,1);
    main_layout->addLayout(central_layout, 1,0, row_len, row_len); 
    main_layout->addWidget(close_button, row_len +1, row_len -1);
    setLayout(main_layout); 
    setWindowTitle(tr("Simulation Constants Menu"));
//connect buttons   
    connect(get_vars, SIGNAL(clicked()), this, SLOT(read_pvars())); 
    connect(set_vars, SIGNAL(stateChanged(int)), this, SLOT(set_write_close(int)));
    connect(close_button, SIGNAL(clicked()), this, SLOT(close())); 
//make menu match proto
    update_menu();
}

pvarMenu::~pvarMenu(){}

void pvarMenu::update_menu() {
    unsigned int i;
    vector<string>::iterator it;

    for(i = 0, it = proto->pnames.begin(); it != proto->pnames.end();i++, it++) {
        central_layout->addWidget(new QLabel(it->c_str()), i, 0);
        update_menu(i);
    }
    QPushButton* add_button = new QPushButton("+");
    central_layout->addWidget(add_button, i, 0, 1, 10);
}

void pvarMenu::update_menu(unsigned int row) {
    unsigned int i;
    int pos = 0;
    int boxlen = 2;
    vector<string>::iterator it;

    for(i = 0, it = proto->pvals[row].begin(); it != proto->pvals[row].end();i++, it++) {
        if(proto->pvals[row][0] == "random") {
            switch(i) {
            case 0: case 1: case 2: {
                QComboBox* options = new QComboBox();
                options->addItems(pvals_options[i]);
                pos = pvals_options[i].indexOf(proto->pvals[row][i].c_str());
                if(pos == -1) {
                    pos = pvals_options[i].length() -1;
                    boxlen = 1;
                    QDoubleSpinBox* logmean = new QDoubleSpinBox();
                    logmean->setValue(atof(proto->pvals[row][i].c_str()));
                    central_layout->addWidget(logmean, row, 2*(i+1), 1, boxlen);
                }
                options->setCurrentIndex(pos);
                central_layout->addWidget(options, row, 2*i +1, 1, boxlen);
                boxlen = 2;
            break; }
            case 3:
                if(proto->pvals[row][2] != "logdistribution") {
                    QDoubleSpinBox* logstdev = new QDoubleSpinBox();
                    logstdev->setValue(atof(proto->pvals[row][i].c_str()));
                    central_layout->addWidget(logstdev, row, 2*i +1, 1, 2);
                }
            break;
            }
        } else if(proto->pvals[row][0] == "iter") {
            if(i == 0) {
                QComboBox* options = new QComboBox();
                options->addItems(pvals_options[i]);
                pos = pvals_options[i].indexOf(proto->pvals[row][i].c_str());
                if(pos != -1) {
                    options->setCurrentIndex(pos);
                }
                central_layout->addWidget(options, row, 2*i+1, 1, 2);
            } else {
                central_layout->addWidget(new QDoubleSpinBox(), row, 2*i+1, 1, 2);
                if(i == 2) {
                    break;
                }
            }
        } else {
            QDoubleSpinBox* init_val = new QDoubleSpinBox();
            QLabel* init_val_label = new QLabel("init value");
            central_layout->addWidget(init_val_label, row, 1, 1, 1);
            central_layout->addWidget(init_val, row, 2, 1, 1);
            break;
        }
        
    }
    QPushButton* remove_button = new QPushButton("-");
    central_layout->addWidget(remove_button, row, 10, 1, 1);
}

void pvarMenu::closeEvent(QCloseEvent* event){
    proto->parmap = proto->resizemap(proto->cell->pars,proto->pnames);
    if(write_close) {
        !(bool)proto->write2Dmap( proto->pnames, proto->pvals, string("pvars") + QDate::currentDate().toString("MMddyy").toStdString() + string(".txt"));
    }
    event->accept();
}

bool pvarMenu::read_pvars(){

    bool ret = false;
    QString fileName = QFileDialog::getOpenFileName(this);
    if (!fileName.isEmpty()){
        proto->measfile = fileName.toStdString();
        ret = proto->parsemixedmap(proto->cell->pars, proto->pvarfile ,&proto->pnames, &proto->pvals);
   }
    update_menu();
    return ret;
}

bool pvarMenu::write_pvars(){
    
    bool ret = false;
    QString fileName = QFileDialog::getOpenFileName(this);
    if (!fileName.isEmpty()){
        proto->measfile = fileName.toStdString();
        ret = !(bool)proto->write2Dmap(proto->pnames, proto->pvals, proto->pvarfile);
    }
    return ret;

}

void pvarMenu::set_write_close(int state) {
    write_close = (bool) state;
    set_vars->setChecked(write_close);
}


