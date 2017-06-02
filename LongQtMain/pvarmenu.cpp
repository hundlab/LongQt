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
#include "pvarmenu.h"

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

    this->pvarsDescriptions = GuiUtils::readMap(":/hoverText/pvarsDescriptions.txt");

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
