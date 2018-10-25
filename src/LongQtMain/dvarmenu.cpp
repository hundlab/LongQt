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
#include "ui_pvarmenu.h"

#include "guiUtils.h"
#include "dvarmenu.h"
using namespace std;
using namespace LongQt;

/*#################################
    begin dvarMenu class
###################################*/
dvarMenu::dvarMenu(shared_ptr<Cell> cell, QWidget *parent): QWidget(parent) {
//setup class variables
    this->cell = cell;
 
    this->createMenu();
}

void dvarMenu::createMenu()  {
    auto type = cell->type();
    QMap<QString, QString> definitions = GuiUtils::concatMaps(GuiUtils::readMap(":/hoverText/dvarsDescriptions.json",type),", ",
                                                              GuiUtils::readMap(":/hoverText/dvarsUnits.json", type), "unitless");
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
    set<string> vars = cell->getVariables();
    QString end_op = "Exit";
    if(parent() != NULL) {
        end_op = "Next";
    }
//initialize layouts and signal maps
    QGridLayout* main_layout = new QGridLayout(this);
    QGridLayout* central_layout = new QGridLayout;
//initialize buttons &lables
    dvars.resize(vars.size());
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
        dvars[i] = new QCheckBox(it->c_str(), this);
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
    main_layout->addLayout(central_layout, 1,0, central_layout->rowCount(), central_layout->columnCount()); 
    setLayout(main_layout); 
    setWindowTitle(tr("Output Variables Menu"));
//make menu match cell 
    update_menu();
}
dvarMenu::~dvarMenu(){}
void dvarMenu::update_menu() {
    set<string>::iterator it;
    unsigned int i; 
    set<string> selection = cell->getVariableSelection();
    set<string> variables = cell->getVariables();
    for(it = variables.begin(), i = 0; it != variables.end(); it++, i++)     {  
        if(selection.find(*it) != selection.end()) {
            dvars[i]->setChecked(true);
        } else {
            dvars[i]->setChecked(false);
        }
    }
}
void dvarMenu::reset() {
    dvars.clear();
    qDeleteAll(this->children());
    createMenu();
}
void dvarMenu::changeCell(shared_ptr<Cell> cell) {
    this->cell = cell;
    this->reset();
}
void dvarMenu::update_datamap(string p, int state){
    set<string> selection = cell->getVariableSelection();
    if((state == 0)) {
        selection.erase(p);
    } else {
        selection.insert(p);
    }
    cell->setVariableSelection(selection);
}
