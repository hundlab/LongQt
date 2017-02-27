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
#include "dvarmenu.h"

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
    QMap<QString, QString> definitions = GuiUtils::concatMaps(GuiUtils::readMap(":/hoverText/dvarsDescriptions.txt"),", ",GuiUtils::readMap(":/hoverText/dvarsUnits.txt"), "unitless");
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
