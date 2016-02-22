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
#include <QToolTip>
#include "varmenu.h"
#include "proto.h"

/*########################
  begin simvarMenu class
##########################*/
simvarMenu::simvarMenu(Protocol* initial_proto, QString init_time, QWidget *parent)  {
//setup class variables
    proto = initial_proto;
    this->parent = parent;
    date_time = init_time;
    write_close = true;
    QMap<QString, QString> descriptions;
    descriptions.insert("bcl","");
    descriptions.insert("meastime", "");
    descriptions.insert("paceflag","");
    descriptions.insert("stimdur","");
    descriptions.insert("tMax","");
    descriptions.insert("maxdoutsize","");
    descriptions.insert("numstims","");
    descriptions.insert("readflag","");
    descriptions.insert("stimt","");
    descriptions.insert("writeflag", "1=write, 0=don't write");
    descriptions.insert("measflag", "");
    descriptions.insert("numtrials","");
    descriptions.insert("saveflag","");
    descriptions.insert("stimval","");
    descriptions.insert("writetime", "time during simulation that writing will begin");

//setup useful constants and aliases
    unsigned int i, row_len = 6;
    map<string,double*> pars = proto->pars;
    map<string,double*>::iterator it;
    QString end_op = "Exit";
    if(parent != NULL) {
        end_op = "Next";
    }
//initialize layouts and signal maps
    QGridLayout* main_layout = new QGridLayout(this);
    QGridLayout* central_layout = new QGridLayout;
    QHBoxLayout* button_group;
//initialize buttons &lables
    simvars = (QDoubleSpinBox**)malloc(pars.size()*sizeof(QDoubleSpinBox*));
    simvar_names = (QLabel**)malloc(pars.size()*sizeof(QLabel*));
    get_vars = new QPushButton(tr("Import settings"), this);
    set_vars = new QCheckBox(QString("Write File on ") += end_op, this);
    close_button = new QPushButton(QString("Save and ") +=end_op, this);
//    QCheckBox readflag = new QCheckBox("Read in variable files", this);
//set button inital states
    if(this->parent != NULL) {
        close_button->hide();
    }
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
        simvar_names[i]->setToolTip(descriptions[it->first.c_str()]);
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
    write_file();
    event->accept();
}
void simvarMenu::write_file() {
    if(write_close) {
        !(bool)proto->writepars(proto->pars, string("simvars") + date_time.toStdString() + string(".txt"));
    }
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
dvarMenu::dvarMenu(Protocol* initial_proto, QString init_time, QWidget *parent)  {
//setup class variables
    proto = initial_proto;
    this->parent = parent;
    date_time = init_time;
    write_close = true;
    QMap<QString, QString> definitions;
        definitions.insert("Gate.d","");
        definitions.insert("Gate.paf","");
        definitions.insert("Gate.r","");
        definitions.insert("cmdnI","");
        definitions.insert("iCart","");
        definitions.insert("iHna","");
        definitions.insert("iTo","Indium Tin Oxide");
        definitions.insert("naI", "Sodium Iodide");
        definitions.insert("Gate.dt","");
        definitions.insert("Gate.pas","");
        definitions.insert("Gate.y","");
        definitions.insert("cmdnR","");
        definitions.insert("iCat","");
        definitions.insert("iKach","");
        definitions.insert("iNak","");
        definitions.insert("iTot", "");
        definitions.insert("t", "Time");
        definitions.insert("Gate.f","");
        definitions.insert("Gate.pi","");
        definitions.insert("caI","");
        definitions.insert("csqn","");
        definitions.insert("iCatt","");
        definitions.insert("iKr","");
        definitions.insert("iNat","");
        definitions.insert("iTr","");
        definitions.insert("trpnCa","");
        definitions.insert("Gate.fca","");
        definitions.insert("Gate.q","");
        definitions.insert("caJsr","");
        definitions.insert("dVdt","");
        definitions.insert("iDiff","");
        definitions.insert("iKs","");
        definitions.insert("iRel","");
        definitions.insert("iTrek","");
        definitions.insert("trpnMg","");
        definitions.insert("Gate.ft","");
        definitions.insert("Gate.qa","");
        definitions.insert("caNsr","");
        definitions.insert("iCait","");
        definitions.insert("iH","");
        definitions.insert("iKt","");
        definitions.insert("iSt","");
        definitions.insert("iUp","");
        definitions.insert("trpnMgmg","");
        definitions.insert("Gate.n","");
        definitions.insert("Gate.qi","");
        definitions.insert("caR","");
        definitions.insert("iCal","");
        definitions.insert("iHk","");
        definitions.insert("iNah","");
        definitions.insert("iSus","");
        definitions.insert("kI","");
        definitions.insert("vOld","");

//setup useful constants and aliases
    unsigned int i, row_len = 6;
    map<string,double*> vars = proto->cell->vars;
    map<string,double*>::iterator it;
    QString end_op = "Exit";
    if(parent != NULL) {
        end_op = "Next";
    }
//initialize layouts and signal maps
    QGridLayout* main_layout = new QGridLayout(this);
    QGridLayout* central_layout = new QGridLayout;
//initialize buttons &lables
    dvars = (QCheckBox**)malloc(vars.size()*sizeof(QCheckBox*));
    get_vars = new QPushButton(tr("Import settings"), this);
    set_vars = new QCheckBox(QString("Write File on ") += end_op, this);
    close_button = new QPushButton(QString("Save and ") +=end_op, this);
//    QCheckBox readflag = new QCheckBox("Read in variable files", this);
//set button inital states
    if(this->parent != NULL) {
        close_button->hide();
    }
    set_vars->setChecked(write_close);
//do all the work for dvars setup
    for(it = vars.begin(), i = 0; it!=vars.end(); it++,i++) {
        pair<string,double*> p (it->first, it->second);
        dvars[i] = new QCheckBox(*(new QString((it->first).c_str())), this);
        central_layout->addWidget(dvars[i], i/row_len, i%row_len);
        connect(dvars[i], &QCheckBox::stateChanged, [=] (int state) {update_datamap(p, state);});
        dvars[i]->setToolTip(definitions[it->first.c_str()]);       //hover attribute
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
    write_file();
    event->accept();
}
void dvarMenu::write_file() {
    if(write_close) {
        !(bool)proto->writedvars(proto->datamap, string("dvars") + date_time.toStdString() + string(".txt"));
    }
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
mvarMenu::mvarMenu(Protocol* initial_proto, QString init_time, QWidget *parent)  {
//setup class variables
    proto = initial_proto;
    this->parent = parent;
    date_time = init_time;
    write_close = true;
//setup useful constants and aliases
    unsigned int row_len = 6;
    std::map<string,double*>::iterator it;
    Measure temp = Measure();
    QString end_op = "Exit";
    if(parent != NULL) {
        end_op = "Next";
    }
//initialize layouts and signal maps
    QGridLayout* main_layout = new QGridLayout(this);
    QGridLayout* central_layout = new QGridLayout;
//initialize buttons &lables
    get_vars = new QPushButton(tr("Import settings"), this);
    set_vars = new QCheckBox(QString("Write File on ") += end_op, this);
    close_button = new QPushButton(QString("Save and ") +=end_op, this);
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
    if(this->parent != NULL) {
        close_button->hide();
    }
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
            vars_view->setToolTip("Details");
        }
    }
    vars_view->setCurrentRow(row);
    if(vars_view->currentRow() >= 0) {
        for(j = 0; j < proto->mpnames[vars_view->currentRow()].size(); j++){
            meas_view->addItem(proto->mpnames[vars_view->currentRow()][j].c_str());
            meas_view->setToolTip("Measure Details");
        }
    }
}
void mvarMenu::closeEvent(QCloseEvent* event){
   write_file();
   event->accept();
}
void mvarMenu::write_file () {
    proto->initializeMeasure(proto->maxmeassize);
    if(write_close) {
        !(bool)proto->write2Dmap( proto->mvnames, proto->mpnames, string("mvars") + date_time.toStdString() + string(".txt"));
    }
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
pvarMenu::pvarMenu(Protocol* initial_proto, QString init_time, QWidget *parent)  {
//setup class variables
    proto = initial_proto;
    this->parent = parent;
    date_time = init_time;
    write_close = true;
    pvals_options = new QStringList[3]();
    pvals_options[0] << "random" << "iter" << "init value";
    pvals_options[1] << "lognormal" << "normal";
    pvals_options[2] << "logdistribution" << "logmean";
//setup useful constants and aliases
    unsigned int row_len = 5;
    std::map<string,double*>::iterator it;
    QString end_op = "Exit";
    if(parent != NULL) {
        end_op = "Next";
    }
//initialize layouts and signal maps
    QGridLayout* main_layout = new QGridLayout(this);
    central_layout = new QGridLayout;
//initialize buttons &lables
    get_vars = new QPushButton(tr("Import settings"), this);
    pvar_table = new QTableWidget(0,5);
    set_vars = new QCheckBox(QString("Write File on ") += end_op, this);
    close_button = new QPushButton(QString("Save and ") +=end_op, this);
//    QCheckBox readflag = new QCheckBox("Read in variable files", this);
//set button inital states
    if(this->parent != NULL) {
        close_button->hide();
    }
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
    unsigned int end_row;
    vector<string>::iterator it;
    map<string, double*>::iterator map_it;
    add_button = new QPushButton("+");
    new_var_choice = new QComboBox();

    for(i = 0, it = proto->pnames.begin(); it != proto->pnames.end();i++, it++) {
        clear_row(i,0);
        central_layout->addWidget(new QLabel(it->c_str()), i, 0);
        update_menu(i);
    }

    for(map_it = proto->cell->pars.begin(); map_it != proto->cell->pars.end(); map_it++) {
        new_var_choice->addItem(map_it->first.c_str());
    }
    end_row = i;
    for(; (int) i<= central_layout->rowCount(); i++) {
        clear_row(i,0);
    }
    central_layout->addWidget(new_var_choice, end_row, 0, 1, 2);
    central_layout->addWidget(add_button, end_row, 2, 1, 2);
    connect(add_button, SIGNAL(clicked()), this, SLOT(add_row()));
}
void pvarMenu::update_menu(unsigned int row) {
    int boxlen = 2;
    int inc_pos = 2;
    vector<string>::iterator it;

    clear_row(row, 1);
        if(proto->pvals[row][0] == "random") {
            add_comobobox_tomenu(row, 0, 0, boxlen);
            add_comobobox_tomenu(row, 1, 1, boxlen);
            if((proto->pvals[row][1] == "lognormal") ) {
                if((proto->pvals[row][2] != "logdistribution")) {
                    boxlen = 1;
                    add_doublespinbox_tomenu(row, 2, boxlen, 6);
                    central_layout->addWidget(new QLabel("logstdev"), row, 7,1,1);
                    add_doublespinbox_tomenu(row, 3, boxlen, 8);
                }
                add_comobobox_tomenu(row, 2, 2, boxlen);
            }
        } else if(proto->pvals[row][0] == "iter") {
               add_comobobox_tomenu(row, 0, 0, 2);
               QCheckBox* take_init = new QCheckBox("inital value"); 
               central_layout->addWidget(take_init, row, 3);
               if(proto->pvals[row].size() == 3) {
                    take_init->setChecked(true);
                    inc_pos = 1;
                    add_doublespinbox_tomenu(row, 1, 1, 4);
               }
               central_layout->addWidget(new QLabel("increment"), row, 5);
               add_doublespinbox_tomenu(row, inc_pos, 1, 6);
               connect(take_init, &QCheckBox::stateChanged,
                    [=](int state){checkbox_changed(row, 1, state);});
        } else {
            add_comobobox_tomenu(row, 0, 0, 1, 1);
            add_doublespinbox_tomenu(row, 0, 1, 2);
        }
        
    QPushButton* remove_button = new QPushButton("-");
    central_layout->addWidget(remove_button, row, 10, 1, 1);
    connect(remove_button, &QPushButton::clicked,
        [=](){
        remove_row(row);}
    );
} 
void pvarMenu::add_doublespinbox_tomenu(unsigned int row, unsigned int column, unsigned int boxlen, int column_pos) {
    if(column_pos < 0) {
        column_pos = 2*column+1;
    }
                QDoubleSpinBox* val = new QDoubleSpinBox();
                val->setRange(std::numeric_limits<int>::min(), std::numeric_limits<int>::max());
                val->setValue(atof(proto->pvals[row][column].c_str()));
                central_layout->addWidget(val, row, column_pos, 1, boxlen);
                connect(val, static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged),
                    [=](double value){
                    proto->pvals[row].erase(proto->pvals[row].begin() + column);
                    proto->pvals[row].insert(proto->pvals[row].begin() +column, to_string(value));});
}
void pvarMenu::add_comobobox_tomenu(unsigned int row, unsigned int column, unsigned int menu_box, unsigned int boxlen, int column_pos) {
    if(column_pos < 0) {
        column_pos = 2*column+1;
    }

    QComboBox* options = new QComboBox();
    int pos =-1;

    options->addItems(pvals_options[menu_box]);
    pos = pvals_options[menu_box].indexOf(proto->pvals[row][column].c_str());
    if(pos == -1) {
        pos = pvals_options[menu_box].length() -1;
    }
    options->setCurrentIndex(pos);
    central_layout->addWidget(options, row, column_pos, 1, boxlen);
    connect(options, static_cast<void (QComboBox::*)(const QString&)>(&QComboBox::currentIndexChanged),
        [=](QString value){row_changed(value, row, column);});
}
void pvarMenu::clear_row(unsigned int row, int offset) {
    unsigned int i;

    for(i = offset; i <= 10;i++) {
        int index = 0;
        QLayoutItem* current = central_layout->itemAtPosition(row, i);
        if(current != 0) {
            current->widget()->setVisible(false);
            index = central_layout->indexOf(current->widget());
            delete central_layout->takeAt(index);
        }

    }
}
void pvarMenu::closeEvent(QCloseEvent* event){
    write_file();
    event->accept();
}
void pvarMenu::write_file() {
    proto->parmap = proto->resizemap(proto->cell->pars,proto->pnames);
    if(write_close) {
        !(bool)proto->write2Dmap( proto->pnames, proto->pvals, string("pvars") + date_time.toStdString() + string(".txt"));
    }
 
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
void pvarMenu::row_changed(QString value, int row, int column) {
    vector<string> to_insert;
    vector<string>::iterator erase_start = proto->pvals[row].begin() +column;
    vector<string>::iterator erase_end = proto->pvals[row].begin() +column;
    vector<string>::iterator new_start = to_insert.begin();
    vector<string>::iterator new_end = to_insert.end();

    if(value == "init value") {
        value = "0.0";
        erase_end = proto->pvals[row].end();
        to_insert.push_back(value.toStdString());
    }
    if(value == "logmean") {
        value = "0.0";
        erase_end = proto->pvals[row].end();
        to_insert.push_back(value.toStdString());
        to_insert.push_back(value.toStdString());

    }
    if(value == "normal" || value == "logdistribution") {
        erase_end = proto->pvals[row].end();
        to_insert.push_back(value.toStdString());
    }
    if(value == "iter") {
        erase_end = proto->pvals[row].end();
        to_insert.push_back(value.toStdString());
        to_insert.push_back("0.0");
        to_insert.push_back("0.0");
    }
    if(value == "random") {
        erase_end = proto->pvals[row].end();
        to_insert.push_back(value.toStdString());
        to_insert.push_back("normal");
    }
    if(value == "lognormal") {
        erase_end = proto->pvals[row].end();
        to_insert.push_back(value.toStdString());
        to_insert.push_back("logdistribution");
    }
    new_start = to_insert.begin();
    new_end = to_insert.end();
    proto->pvals[row].erase(erase_start,erase_end);
    proto->pvals[row].insert(proto->pvals[row].begin() +column, new_start, new_end);
    update_menu(row);
}
void pvarMenu::remove_row(unsigned int row) {
    proto->pvals.erase(proto->pvals.begin() + row);
    proto->pnames.erase(proto->pnames.begin() + row);
    clear_row(row, 0);
    update_menu();
}
void pvarMenu::add_row() {
    vector<string>* def = new vector<string>();
    vector<string>::iterator it;
    bool found = false;
    string var_name_to_add = new_var_choice->currentText().toStdString();

    for(it = proto->pnames.begin(); it != proto->pnames.end(); it++) {
        if(*it == var_name_to_add) {
            found = true;
        }
    }

    if(!found) {
        def->push_back("random");
        def->push_back("normal");
        proto->pnames.push_back(var_name_to_add);
        proto->pvals.push_back(*def);
        update_menu();
    }
}
void pvarMenu::checkbox_changed(unsigned int row,unsigned int column,int state) {
    switch(state) {
    case 0:
        proto->pvals[row].erase(proto->pvals[row].begin() + column);
    break;
    case 2:
        proto->pvals[row].insert(proto->pvals[row].begin() + column, "0.0");
    break;
    }
    update_menu(row);
}
