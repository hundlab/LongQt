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
#include <QFormLayout>
#include <QTabWidget>
#include <QList>

#include "varmenu.h"
#include "protocol.h"
#include "gridSettup.h"
#include "guiUtils.h"


/*########################
  begin simvarMenu class
##########################*/
simvarMenu::simvarMenu(Protocol* initial_proto, QDir working_dir, QWidget *parent)  {
//setup class variables
    proto = initial_proto;
    this->parent = parent;
    this->working_dir = working_dir;
    write_close = true;
    descriptions = GuiUtils().readMap(":/hoverText/parsDescriptions.txt");
    this->createMenu();
}

void simvarMenu::createMenu()  {
//setup useful constants and aliases
    QString end_op = "Exit";
    if(parent != NULL) {
        end_op = "Next";
    }
//initialize layouts
    main_layout = new QGridLayout(this);
    QList<QHBoxLayout*> central_layouts;
//initialize buttons &lables
    QTabWidget* tabs = new QTabWidget();
    get_vars = new QPushButton(tr("Import Simulation settings"), this);
    set_vars = new QCheckBox(QString("Write File on ") += end_op, this);
    close_button = new QPushButton(QString("Save and ") +=end_op, this);
//    QCheckBox readflag = new QCheckBox("Read in variable files", this);
//set button inital states
    if(this->parent != NULL) {
        close_button->hide();
    }
    set_vars->setChecked(write_close);
    
//do all the work for simvars setup
    for(map<string,GetSetRef>::iterator it = proto->pars.begin(); it!=proto->pars.end(); it++) {
        if(simvars_layouts.find(it->second.type.c_str()) == simvars_layouts.end()) {
            simvars_layouts.insert(it->second.type.c_str(), new QFormLayout());
        }
        initialize(it);
    }
    central_layouts.push_back(new QHBoxLayout());
    if(simvars_layouts["double"] != NULL) {
        central_layouts.last()->addLayout(simvars_layouts["double"]);
    }
    if(simvars_layouts["int"] != NULL) {
        central_layouts.last()->addLayout(simvars_layouts["int"]);
    }
    if(simvars_layouts["bool"] != NULL) {
        central_layouts.last()->addLayout(simvars_layouts["bool"]);
    }
    central_layouts.last()->addLayout(simvars_layouts["cell"]);
    central_layouts.push_back(new QHBoxLayout());
    central_layouts.last()->addLayout(simvars_layouts["file"]);
    central_layouts.last()->addLayout(simvars_layouts["directory"]);
//tabs settup
    {
    QWidget* temp = new QWidget();
    temp->setLayout(central_layouts.first());
    tabs->addTab(temp,"Simulation Variables");
    temp = new QWidget();
    temp->setLayout(central_layouts.last());
    tabs->addTab(temp,"Simulation files");
    if(proto->pars["celltype"].get() == "gridCell") {
        gridSetupWidget* grid = new gridSetupWidget((gridProtocol*)this->proto,working_dir);
        tabs->addTab(grid, "Grid Setup");
        connect(grid, &gridSetupWidget::cell_type_changed, this, &simvarMenu::cell_type_changed);
        connect(this, &simvarMenu::updated, grid, &gridSetupWidget::updateMenu);
    }
    }
//main_layout
    main_layout->addWidget(get_vars, 0,0);
    main_layout->addWidget(set_vars, 0,1);
    main_layout->addWidget(tabs , 1,0,10 ,19 ); 
    main_layout->addWidget(close_button, 11, 20);
    setWindowTitle(tr("Simulation Variables Menu"));
    setLayout(main_layout);
//connect buttons   
    connect(get_vars, SIGNAL(clicked()), this, SLOT(read_simvars())); 
    connect(set_vars, SIGNAL(stateChanged(int)), this, SLOT(set_write_close(int)));
    connect(close_button, SIGNAL(clicked()), this, SLOT(close())); 
//make menu match proto
    update_menu();
}
void simvarMenu::initialize(const map<string,GetSetRef>::iterator it) {
    QMap<QString,function<void(const map<string,GetSetRef>::iterator)>> initializers;
    initializers["double"] = [this] (const map<string, GetSetRef>::iterator it) {
        QDoubleSpinBox* new_simvar = new QDoubleSpinBox();
        QLabel* simvars_label = new QLabel(*(new QString((it->first).c_str())));
        simvars_label->setToolTip(descriptions[(it->first).c_str()]);
        string name = it->first;
        new_simvar->setRange(/*std::numeric_limits<double>::min()*/ -100000,std::numeric_limits<double>::max());
        simvars.insert(it->first.c_str(), new_simvar);
        simvars_layouts[it->second.type.c_str()]->addRow(simvars_label, new_simvar);
        connect((QDoubleSpinBox*)simvars.last(), static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), [=] (double value) {update_pvars(pair<string,double>(name, value));});
    };
    initializers["int"] = [this] (const map<string, GetSetRef>::iterator it) {
        QSpinBox* new_simvar = new QSpinBox();
        QLabel* simvars_label = new QLabel(*(new QString((it->first).c_str())));
        simvars_label->setToolTip(descriptions[(it->first).c_str()]);
        string name = it->first;
        new_simvar->setRange(/*std::numeric_limits<int>::min()*/-100000, std::numeric_limits<int>::max());
        simvars.insert(it->first.c_str(),new_simvar);
        simvars_layouts[it->second.type.c_str()]->addRow(simvars_label, new_simvar);
        connect((QSpinBox*)simvars.last(), static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), [=] (int value) {update_pvars(pair<string,int>(name, value));});
    };
    initializers["bool"] = [this] (const map<string, GetSetRef>::iterator it) {
        QCheckBox* new_simvar = new QCheckBox();
        QLabel* simvars_label = new QLabel(*(new QString((it->first).c_str())));
        simvars_label->setToolTip(descriptions[(it->first).c_str()]);
        string name = it->first;
        string type = it->second.type;
        simvars.insert(it->first.c_str(),new_simvar);
        simvars_layouts[it->second.type.c_str()]->addRow(simvars_label, new_simvar);
        connect((QCheckBox*)simvars.last(), static_cast<void (QCheckBox::*)(int)>(&QCheckBox::stateChanged), [=] (int value) {update_pvars(pair<string,int>(name, value), type);});
    };
    initializers["file"] = [this] (const map<string, GetSetRef>::iterator it) {
        QLineEdit* new_simvar = new QLineEdit();
        QLabel* simvars_label = new QLabel(*(new QString((it->first).c_str())));
        simvars_label->setToolTip(descriptions[(it->first).c_str()]);
        string name = it->first;
        string type = it->second.type;
        new_simvar->setReadOnly(true);
        simvars.insert(it->first.c_str(),new_simvar);
        simvars_layouts[it->second.type.c_str()]->addRow(simvars_label, new_simvar);
        connect((QLineEdit*)simvars.last(), static_cast<void (QLineEdit::*)(const QString&)>(&QLineEdit::textEdited), [=] (QString value) {update_pvars(pair<string,string>(name, value.toStdString()), type);});
    };   
    initializers["directory"] = [this] (const map<string, GetSetRef>::iterator it) {
        QLineEdit* new_simvar = new QLineEdit();
        QPushButton* setDir = new QPushButton("Choose");
        QWidget* layoutWidget = new QWidget();
        QHBoxLayout* layout = new QHBoxLayout(layoutWidget);
        QLabel* simvars_label = new QLabel(*(new QString((it->first).c_str())));
        layout->setContentsMargins(0,0,0,0);
        layout->addWidget(new_simvar);
        layout->addWidget(setDir);
        simvars_label->setToolTip(descriptions[(it->first).c_str()]);
        string name = it->first;
        string type = it->second.type;
        simvars.insert(it->first.c_str(),new_simvar);
        simvars_layouts[it->second.type.c_str()]->addRow(simvars_label, layoutWidget);
        connect(new_simvar, static_cast<void (QLineEdit::*)(const QString&)>(&QLineEdit::textEdited), [=] (QString value) {update_pvars(pair<string,string>(name, value.toStdString()), type);});
        connect(setDir, &QPushButton::clicked, [=] () {
            QString value = QFileDialog::getExistingDirectory(this,tr(QString(it->first.c_str()).toStdString().c_str()),it->second.get().c_str());
            if(value != "") {
                this->update_pvars(pair<string,string>(name, value.toStdString()), type);
				this->update_menu();
            }
        });
    };
    initializers["cell"] = [this]  (const map<string, GetSetRef>::iterator it) {
        QStringList cell_options;
        auto cell_type = new QComboBox();
        QLabel* simvars_label = new QLabel(*(new QString((it->first).c_str())));
        auto cell_opitons_stl = proto->cellOptions();
        for(auto im = cell_opitons_stl.begin(); im != cell_opitons_stl.end(); im++) {
            cell_options << im->c_str();
        }
        cell_type->addItems(cell_options);
        string name = it->first;
        string type = it->second.type;
        set_default_vals(proto->pars[it->first].get());
        simvars.insert(it->first.c_str(),cell_type);
        simvars_layouts[it->second.type.c_str()]->addRow(simvars_label, cell_type);
        connect((QComboBox*)simvars.last(), static_cast<void (QComboBox::*)(const QString&)>(&QComboBox::currentIndexChanged), [=] (QString value) {update_pvars(pair<string,string>(name, value.toStdString()), type);});
    };

    try {
        initializers[it->second.type.c_str()](it);
    } catch(std::bad_function_call e){}
}
simvarMenu::~simvarMenu(){}
void simvarMenu::update_menu() {
    QMap<QString,function<void(const map<string, GetSetRef>::iterator)>> updaters;
    updaters["double"] = [this] (const map<string, GetSetRef>::iterator it)
         {string s =it->second.get() ;((QDoubleSpinBox*)simvars[it->first.c_str()])->setValue(std::stod(s));};
    updaters["int"] = [this] (const map<string, GetSetRef>::iterator it)
         {((QSpinBox*)simvars[it->first.c_str()])->setValue(std::stoi(it->second.get()));};
    updaters["bool"] = [this] (const map<string, GetSetRef>::iterator it)
         {((QCheckBox*)simvars[it->first.c_str()])->setChecked(Protocol::stob(it->second.get()));};
    updaters["file"] = [this] (const map<string, GetSetRef>::iterator it) {
        QLineEdit* line = (QLineEdit*)simvars[it->first.c_str()];
        if(line->text() != QString(it->second.get().c_str())) {
            line->setText(it->second.get().c_str());
        }
    };
    updaters["directory"] = [this] (const map<string, GetSetRef>::iterator it) {
        QLineEdit* line = (QLineEdit*)simvars[it->first.c_str()];
        if(line->text() != QString(it->second.get().c_str())) {
            line->setText(it->second.get().c_str());
        }
    };
    updaters["cell"] = [this] (const map<string, GetSetRef>::iterator it)
         {  QComboBox* simv = ((QComboBox*)simvars[it->first.c_str()]);
            int index = simv->findText(it->second.get().c_str());
            if(index != -1) {
                simv->setCurrentIndex(index);
            }
         };

    for(auto it = proto->pars.begin(); it != proto->pars.end(); it++){
        try {
            updaters[it->second.type.c_str()](it);
        } catch(std::bad_function_call e){}
    }
    emit updated();
}
void simvarMenu::reset() {
    simvars.clear();
    simvars_layouts.clear();
    qDeleteAll(this->children());
    createMenu();
}
void simvarMenu::changeProto(Protocol* proto) {
    this->proto = proto;
    this->reset();
}
void simvarMenu::closeEvent(QCloseEvent* event){
    write_file();
    event->accept();
}
void simvarMenu::write_file() {
    if(write_close) {
        working_dir.mkpath(working_dir.absolutePath());
        proto->writepars(working_dir.absolutePath().toStdString() + string("/") + proto->simvarfile);
    }
}
void simvarMenu::setWorkingDir(QDir& dir) {
    working_dir = dir;
    update_menu();
}
bool simvarMenu::read_simvars(){
    bool ret = false;
    QString fileName = QFileDialog::getOpenFileName(this,"Simulation Settings",working_dir.absolutePath());
    if (!fileName.isEmpty()){
        ret = !(bool)proto->readpars(fileName.toStdString());
		try {
			if(proto->pars.at("writeCellState").get() == "true") {
				proto->pars.at("cellStateFile").set(fileName.toStdString());
				proto->pars.at("readCellState").set("true");
				proto->pars.at("writeCellState").set("false");
			}
		} catch (const std::out_of_range&) {
		}
    }
    proto->datadir = working_dir.absolutePath().toStdString();
    update_menu();
    return ret;
}
bool simvarMenu::write_simvars(){
    
    bool ret = false;
    QString fileName = QFileDialog::getOpenFileName(this);
    if (!fileName.isEmpty()){
        proto->simvarfile = fileName.toStdString();
    ret = !(bool)proto->writepars(proto->simvarfile);
    }
    return ret;

}
void simvarMenu::update_pvars(pair<string,double> p){
    proto->pars.at(p.first).set(to_string(p.second));
}
void simvarMenu::update_pvars(pair<string,string> p, string type){
     if(type == "cell") {
        proto->pars[p.first].set(p.second);
        set_default_vals(p.second);
        emit cell_type_changed();
     } else {
        proto->pars[p.first].set(p.second);
     }
     if(p.first == "datadir") {
        working_dir = QString(p.second.c_str());
        emit working_dir_changed(working_dir);
     }
}
void simvarMenu::update_pvars(pair<string,int> p, string type) {
    if(type == "bool") {
        proto->pars[p.first].set(Protocol::to_string(p.second));
    } else {
        proto->pars[p.first].set(to_string(p.second));
    }
}
void simvarMenu::set_write_close(int state) {
    write_close = (bool) state;
    set_vars->setChecked(write_close);
}
void simvarMenu::changeCellType() {
    update_menu(); 
}
void simvarMenu::set_default_vals(string name) {
    auto cellDefaultsList = cellUtils().protocolCellDefaults[name];
    for(auto val : cellDefaultsList) {
        try {
            proto->pars.at(val.first).set(val.second);
        } catch(bad_function_call) {
        } catch(out_of_range) {};
    }
}
/*#################################
    begin dvarMenu class
###################################*/
dvarMenu::dvarMenu(Protocol* initial_proto, QDir working_dir, QWidget *parent)  {
//setup class variables
    proto = initial_proto;
    this->parent = parent;
    this->working_dir = working_dir;
    write_close = true;
 
    this->createMenu();
}

void dvarMenu::createMenu()  {
    QMap<QString, QString> definitions = GuiUtils().concatMaps(GuiUtils().readMap(":/hoverText/dvarsDescriptions.txt"),", ",GuiUtils().readMap(":/hoverText/dvarsUnits.txt"), "unitless");
/*    for(auto temp = definitions.begin(); temp != definitions.end();temp++) {
    qInfo() << temp.key();
    qInfo() << temp.value();
    }*/
    QMap<QString,QRegExp> dvars_groups;
        dvars_groups.insert("Gates",QRegExp("^Gate.",Qt::CaseInsensitive));
        dvars_groups.insert("Currents",QRegExp("^i"));
        dvars_groups.insert("Calcium Concentrations", QRegExp("^ca"));
 
 //setup useful constants and aliases
    unsigned int i, row_len = 6;
    set<string> vars = proto->cell->getVariables();
    QString end_op = "Exit";
    if(parent != NULL) {
        end_op = "Next";
    }
//initialize layouts and signal maps
    QGridLayout* main_layout = new QGridLayout(this);
    QGridLayout* central_layout = new QGridLayout;
//initialize buttons &lables
    dvars = (QCheckBox**)malloc(vars.size()*sizeof(QCheckBox*));
    get_vars = new QPushButton(tr("Import Tracked Variable Settings"), this);
    set_vars = new QCheckBox(QString("Write File on ") += end_op, this);
    close_button = new QPushButton(QString("Save and ") +=end_op, this);
//    QCheckBox readflag = new QCheckBox("Read in variable files", this);
//set button inital states
    if(this->parent != NULL) {
        close_button->hide();
    }
    set_vars->setChecked(write_close);
//do all the work for dvars setup
    QMap<QString,QList<QCheckBox*>> widgets;
    for(auto it = dvars_groups.begin(); it != dvars_groups.end(); it++) {
        widgets.insert(it.key(), QList<QCheckBox*>());
    }
    widgets.insert("Other", QList<QCheckBox*>());
    {
    auto it = vars.begin();
    for(i = 0; it!=vars.end(); it++,i++) {
        bool found = false;
        string p =  *it;
        dvars[i] = new QCheckBox(*(new QString(it->c_str())), this);
        for(auto im = dvars_groups.begin(); im != dvars_groups.end(); im++) {
            if(im->indexIn(p.c_str()) != -1) {
                found = true;
                widgets[im.key()].append(dvars[i]);
                break;
            }
        }
        if(!found) {
            widgets["Other"].append(dvars[i]);
        }
//        central_layout->addWidget(dvars[i], i/row_len, i%row_len);
        connect(dvars[i], &QCheckBox::stateChanged, [=] (int state) {update_datamap(p, state);});
        if(*it == "t" || *it == "vOld") {
            dvars[i]->setEnabled(false);
        }
        dvars[i]->setToolTip(definitions[it->c_str()]);       //hover attribute
    }
    }
    {
    int currRow = 1;
    int j = 0;
    for(auto it = widgets.begin(); it != widgets.end(); it++) {
        QGridLayout* layout = new QGridLayout();
        QGroupBox* box = new QGroupBox(it.key());
        std::sort(it.value().begin(), it.value().end(), [] (QCheckBox* a, QCheckBox* b) {
            return a->text() <= b->text();
        });
        for(auto im = it.value().begin(); im != it.value().end(); im++) {
            layout->addWidget(*im, j/row_len, j%row_len);
            j++;
        }
        box->setLayout(layout);
        if(!it.value().isEmpty()) {
            central_layout->addWidget(box, currRow, 0, layout->rowCount(),  layout->columnCount());
        }
        currRow += layout->rowCount()+1;
        j = 0;
    }
    }
//main_layout
    main_layout->addWidget(get_vars, 0,0);
    main_layout->addWidget(set_vars, 0,1);
    main_layout->addLayout(central_layout, 1,0, central_layout->rowCount(), central_layout->columnCount()); 
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
    set<string>::iterator it;
    unsigned int i; 
    set<string> selection = proto->cell->getVariableSelection();
    set<string> variables = proto->cell->getVariables();
    for(it = variables.begin(), i = 0; it != variables.end(); it++, i++)     {  
        if(selection.find(*it) != selection.end()) {
            dvars[i]->setChecked(true);
        } else {
            dvars[i]->setChecked(false);
        }
    }
}
void dvarMenu::reset() {
    free(dvars);
    qDeleteAll(this->children());
    createMenu();
}
void dvarMenu::changeProto(Protocol* proto) {
    this->proto = proto;
    this->reset();
}
void dvarMenu::closeEvent(QCloseEvent* event){
    write_file();
    event->accept();
}
void dvarMenu::write_file() {
    if(write_close) {
        working_dir.mkpath(working_dir.absolutePath());
        proto->writedvars(working_dir.absolutePath().toStdString() + string("/") + proto->dvarfile);
    }
}
void dvarMenu::setWorkingDir(QDir& dir) {
    working_dir = dir;
}
bool dvarMenu::read_dvars(){
    bool ret = false;
    QString fileName = QFileDialog::getOpenFileName(this,"Tracked Variable Settings",working_dir.absolutePath());
    if (!fileName.isEmpty()){
        ret = proto->readdvars(fileName.toStdString());  // use names in dvars.txt to resize datamap
    }
    update_menu();
    return ret;
}
bool dvarMenu::write_dvars(){  
    bool ret = false;
    QString fileName = QFileDialog::getOpenFileName(this);
    if (!fileName.isEmpty()){
        proto->dvarfile = fileName.toStdString();
    ret = !(bool)proto->writedvars(proto->dvarfile);
    }
    return ret;

}
void dvarMenu::update_datamap(string p, int state){
    set<string> selection = proto->cell->getVariableSelection();
    if((state == 0)) {
        selection.erase(p);
    } else {
        selection.insert(p);
    }
    proto->cell->setVariableSelection(selection);
}
void dvarMenu::set_write_close(int state) {
    write_close = (bool) state;
    set_vars->setChecked(write_close);
}
/*#################################
    begin mvarMenu class
###################################*/
mvarMenu::mvarMenu(Protocol* initial_proto, QDir working_dir, QWidget *parent)  {
//setup class variables
    proto = initial_proto;
    this->parent = parent;
    this->working_dir = working_dir;
    write_close = true;

    this->dvarsDescriptions = GuiUtils().concatMaps(GuiUtils().readMap(":/hoverText/dvarsDescriptions.txt"),", ",GuiUtils().readMap(":/hoverText/dvarsUnits.txt"));
    this->measDescriptions = GuiUtils().readMap(":/hoverText/measDescriptions.txt");
    this->createMenu();
}

void mvarMenu::createMenu()  {
//setup useful constants and aliases
    unsigned int row_len = 6;
    Measure temp = Measure();
    QString end_op = "Exit";
    if(parent != NULL) {
        end_op = "Next";
    }
//initialize layouts and signal maps
    QGridLayout* main_layout = new QGridLayout(this);
    QGridLayout* central_layout = new QGridLayout;
//initialize buttons &lables
    get_vars = new QPushButton(tr("Import Measurement Settings"), this);
    set_vars = new QCheckBox(QString("Write File on ") += end_op, this);
    close_button = new QPushButton(QString("Save and ") +=end_op, this);
    vars_view = new QListWidget(this);
    meas_view = new QListWidget(this);
    meas_list_label = new QLabel("Aspects to Measure",this);
    vars_list_label = new QLabel("Variables to Track", this);
    addto_vars_options = new QComboBox(this);
    addto_meas_options = new QComboBox(this);
    addto_meas_list_button = new QPushButton("+", this);
    removefr_meas_list_button = new QPushButton("-", this);
    addto_vars_list_button = new QPushButton("+", this);
    removefr_vars_list_button = new QPushButton("-", this);
    QLabel* percrepol_label = new QLabel("Percent Repolarization",this);
    percrepol_spinbox = new QSpinBox(this);
//    QCheckBox readflag = new QCheckBox("Read in variable files", this);
//set button inital states
    measure_options = temp.getVariables();
    if(this->parent != NULL) {
        close_button->hide();
    }
    set_vars->setChecked(write_close);
    int i =0;
    set<string> cellVars = proto->cell->getVariables();
    for(auto it = cellVars.begin(); it != cellVars.end(); it++) {
        addto_vars_options->addItem(it->c_str());
        addto_vars_options->setItemData(i,dvarsDescriptions[it->c_str()],Qt::ToolTipRole);
        i++;
    }
    this->checkMeasOpts(this->addto_vars_options->currentText());
    vars_view->setSortingEnabled(true);
    meas_view->setSortingEnabled(true);
    percrepol_spinbox->setRange(0,99);
    percrepol_spinbox->setValue((int)temp.getPercrepol());
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
    central_layout->addWidget(percrepol_label,4,0);
    central_layout->addWidget(percrepol_spinbox,4,1);
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
    connect(addto_vars_options, &QComboBox::currentTextChanged, this, &mvarMenu::checkMeasOpts);
    connect(addto_vars_list_button, &QPushButton::clicked, this, &mvarMenu::addto_vars_list);
    connect(removefr_vars_list_button, &QPushButton::clicked, this, &mvarMenu::removefr_vars_list);
    connect(addto_meas_list_button, &QPushButton::clicked, this, &mvarMenu::addto_meas_list);
    connect(removefr_meas_list_button, &QPushButton::clicked, this, &mvarMenu::removefr_meas_list);
    connect(set_vars, SIGNAL(stateChanged(int)), this, SLOT(set_write_close(int)));
    connect(close_button, SIGNAL(clicked()), this, SLOT(close())); 
    connect(percrepol_spinbox, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), [this] (int val) {auto measures = this->proto->Measures;
		    for(auto& meas : measures) {
		    	meas.second.setPercrepol(val);
		    }
		    this->proto->setMeasures(measures);
		    });
//make menu match proto
    update_menu(-1);
}
mvarMenu::~mvarMenu(){}
void mvarMenu::update_menu(int row) {
    for(auto it = proto->Measures.begin(); it != proto->Measures.end(); it++) {
        QList<QListWidgetItem*> vars_item = vars_view->findItems(it->second.varname.c_str(),Qt::MatchExactly);
        if(vars_item.empty()) {
            QListWidgetItem* to_add = new QListWidgetItem(it->second.varname.c_str());
            to_add->setToolTip(dvarsDescriptions[it->second.varname.c_str()]);
            vars_view->addItem(to_add);
            row = vars_view->row(vars_view->findItems(it->second.varname.c_str(),Qt::MatchExactly).first());
        }
    }

    meas_view->clear();
    vars_view->setCurrentRow(row);
    if(vars_view->currentRow() >= 0) {
        auto selection = proto->Measures.at(vars_view->currentItem()->text().toStdString()).Selection;
        for(auto it = selection.begin(); it != selection.end(); it++){
            QList<QListWidgetItem*> meas_item = meas_view->findItems(it->c_str(),Qt::MatchExactly);
            if(meas_item.empty()) {
                QListWidgetItem* to_add = new QListWidgetItem(it->c_str());
                to_add->setToolTip(measDescriptions[it->c_str()]);
                meas_view->addItem(to_add);
            }
        }
    }
	if(proto->Measures.size() >0) {
		double val = proto->Measures.begin()->second.getPercrepol();
		if(val != this->percrepol_spinbox->value()) {
			this->percrepol_spinbox->setValue(val);
			auto measures = this->proto->Measures;
		    for(auto& meas : measures) {
		    	meas.second.setPercrepol(val);
		    }
		}
	}
}
void mvarMenu::reset() {
    qDeleteAll(this->children());
    createMenu();
}
void mvarMenu::changeProto(Protocol* proto) {
    this->proto = proto;
    this->reset();
}
void mvarMenu::closeEvent(QCloseEvent* event){
   write_file();
   event->accept();
}
void mvarMenu::write_file () {
    if(write_close) {
        working_dir.mkpath(working_dir.absolutePath());
        proto->writeMVarsFile(working_dir.absolutePath().toStdString() + string("/") + proto->measfile);
    }
}
void mvarMenu::setWorkingDir(QDir& dir) {
    working_dir = dir;
}
bool mvarMenu::read_mvars(){
    bool ret = false;
    QString fileName = QFileDialog::getOpenFileName(this,"Measurement Settings",working_dir.absolutePath());
    if (!fileName.isEmpty()){
        ret = proto->readMvarsFile(fileName.toStdString());
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
        ret = proto->writeMVarsFile(proto->measfile.c_str());
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
    QString measure_name;
    if(vars_view->currentItem() == NULL) {
        this->addto_vars_list();
        return;
    }
    measure_name = vars_view->currentItem()->text();
    if(to_add != "peak" && (measure_name != "vOld"  && measure_name != "caI")) {
        return;
    }
//    if(meas_view->findItems(to_add, Qt::MatchExactly).empty()) {
        proto->addToMeasreSelection(measure_name.toStdString(), to_add.toStdString());
//    }
    update_menu(var_row);
};
void mvarMenu::removefr_meas_list(){
    int meas_row = meas_view->currentRow();
    int var_row = vars_view->currentRow();
    QListWidgetItem* taken_meas;

    if((taken_meas = meas_view->takeItem(meas_row)) == 0) {
        return;
    }
    if(meas_view->count() == 0) {
        removefr_vars_list();
        return;
    }

    meas_view->setCurrentRow(meas_row -1);
    if(var_row != -1) {
        proto->removeFromMeasureSelection(vars_view->currentItem()->text().toStdString(), taken_meas->text().toStdString());
        update_menu(vars_view->currentRow());
    }
};
void mvarMenu::addto_vars_list(){
    QString to_add = addto_vars_options->currentText();

    if(vars_view->findItems(to_add, Qt::MatchExactly).empty()) {
        proto->addMeasure(Measure(to_add.toStdString(), (int)percrepol_spinbox->value()));
        update_menu(-1);
        addto_meas_list();
    }
//    update_menu(proto->mpnames.size() -1);
};
void mvarMenu::removefr_vars_list(){
    int var_row = vars_view->currentRow();
    QListWidgetItem* item = vars_view->takeItem(var_row);
    if(item == 0) {
        return;
    }
    proto->removeMeasure(item->text().toStdString());
    update_menu(var_row -1);
};
void mvarMenu::switch_var(int row){
    update_menu(row);
};
void mvarMenu::checkMeasOpts(const QString& value) {
    bool fullList = (value == "vOld" || value == "caI");
    this->addto_meas_options->clear();
    if(fullList) {
        int i =0;
        for(set<string>::iterator set_it = this->measure_options.begin(); set_it != this->measure_options.end(); set_it++) {
            addto_meas_options->addItem(set_it->c_str());
            addto_meas_options->setItemData(i,measDescriptions[set_it->c_str()],Qt::ToolTipRole);
            i++;
        }
    } else {
        addto_meas_options->addItem("peak");
        addto_meas_options->setItemData(0,measDescriptions["peak"],Qt::ToolTipRole);
    }
};

/*#################################
    begin pvarMenu class
###################################*/
pvarMenu::pvarMenu(Protocol* initial_proto, QDir working_dir, QWidget *parent)  {
//setup class variables
    proto = initial_proto;
    this->parent = parent;
    this->working_dir = working_dir;
    write_close = true;
    pvals_options = new QStringList[3]();
    pvals_options[0] << "random" << "iter" << "init value";
    pvals_options[1] << "lognormal" << "normal";
    pvals_options[2] << "logdistribution" << "logmean";

    this->pvarsDescriptions = GuiUtils().readMap(":/hoverText/pvarsDescriptions.txt");

    this->createMenu();
}

void pvarMenu::createMenu()  {
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
    get_vars = new QPushButton(tr("Import Initializer Settings"), this);
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
    QRegExp* allowed_vars = new QRegExp("Factor");

    for(i = 0, it = proto->pnames.begin(); it != proto->pnames.end();i++, it++) {
        clear_row(i,0);
        QLabel* newLabel = new QLabel(it->c_str());
        newLabel->setToolTip(pvarsDescriptions[it->c_str()]);
        central_layout->addWidget(newLabel, i, 0);
        update_menu(i);
    }

    for(map_it = proto->cell->pars.begin(); map_it != proto->cell->pars.end(); map_it++) {
        QString to_insert = map_it->first.c_str();
        if(allowed_vars->indexIn(to_insert) != -1) {
            new_var_choice->addItem(to_insert);
            new_var_choice->setItemData(new_var_choice->count()-1,pvarsDescriptions[to_insert],Qt::ToolTipRole);
        }
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
                    inc_pos = 2;
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
void pvarMenu::reset() {
    qDeleteAll(this->children());
    createMenu();
}
void pvarMenu::changeProto(Protocol* proto) {
    this->proto = proto;
    this->reset();
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
    if(write_close) {
        working_dir.mkpath(working_dir.absolutePath());
        proto->write2Dmap( proto->pnames, proto->pvals,working_dir.absolutePath().toStdString() + string("/") + proto->pvarfile);
    }
 
}
void pvarMenu::setWorkingDir(QDir& dir) {
    working_dir = dir;
}
bool pvarMenu::read_pvars(){

    bool ret = false;
    QString fileName = QFileDialog::getOpenFileName(this,"Initializer Settings",working_dir.absolutePath());
    if (!fileName.isEmpty()){
        ret = proto->parsemixedmap(proto->cell->pars, fileName.toStdString() ,&proto->pnames, &proto->pvals);
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
    if(var_name_to_add == "") {
        found = true;
    }
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
