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

#include "guiUtils.h"
#include "simvarmenu.h"

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
        this->grid = new gridSetupWidget((gridProtocol*)this->proto,working_dir);
        tabs->addTab(grid, "Grid Setup");
        connect(grid, &gridSetupWidget::cell_type_changed, this, &simvarMenu::cell_type_changed);
//        connect(this, &simvarMenu::updated, grid, &gridSetupWidget::updateMenu);
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
				if(!this->signalCellTypeChange) {
					bool oldState = simv->blockSignals(true);
                	simv->setCurrentIndex(index);
					simv->blockSignals(oldState);
				} else {
                	simv->setCurrentIndex(index);
				}
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
	if(this->grid) {
		this->grid->reset();
	}
	this->signalCellTypeChange = false;
    update_menu();
	this->signalCellTypeChange = true;
	emit cell_type_changed();
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
