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
#include <QRadioButton>
#include <gridsetupwidget.h>

#include "guiUtils.h"
#include "simvdouble.h"
#include "simvint.h"
#include "simvbool.h"
#include "simvfile.h"
#include "simvdir.h"
#include "simvcell.h"
#include "simvcellopts.h"
#include "simvarmenu.h"
#include "ui_simvarmenu.h"

SimvarMenu::SimvarMenu(shared_ptr<Protocol> initial_proto, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SimvarMenu)
{
    ui->setupUi(this);
    //setup class variables
        proto = initial_proto;
        descriptions = GuiUtils::readMap(":/hoverText/parsDescriptions.txt");
        this->createMenu();
}

SimvarMenu::~SimvarMenu()
{
    this->grid->deleteLater();
    this->grid = 0;
    delete ui;
}

void SimvarMenu::createMenu()  {
    QMap<QString,QFormLayout*> simvars_layouts;
    map<string,SimvarInitializer> initializers = {
        {"double",[] (shared_ptr<Protocol> proto,string name) {return new SimvDouble(proto,name);}},
        {"int",[] (shared_ptr<Protocol> proto,string name) {return new SimvInt(proto,name);}},
        {"bool",[] (shared_ptr<Protocol> proto,string name) {return new SimvBool(proto,name);}},
        {"file",[] (shared_ptr<Protocol> proto,string name) {return new SimvFile(proto,name);}},
        {"directory",[] (shared_ptr<Protocol> proto,string name) {return new SimvDir(proto,name);}},
        {"cell",[] (shared_ptr<Protocol> proto,string name) {return new SimvCell(proto,name);}},
        {"cell_option",[] (shared_ptr<Protocol> proto,string name) {return new SimvCellOpts(proto,name);}}
    };
    //do all the work for simvars setup
    for(auto& it : proto->pars) {
        auto type = it.second.type;
        if(simvars_layouts.find(type.c_str()) == simvars_layouts.end()) {
            simvars_layouts.insert(type.c_str(), new QFormLayout());
        }
        QLabel* simvars_label = new QLabel((it.first).c_str());
        simvars_label->setToolTip(descriptions[(it.first).c_str()]);
        try {
            auto widg = initializers.at(type)(proto,it.first);
            widg->setObjectName(it.first.c_str());
            simvars.insert(it.first.c_str(), widg);
            simvars_layouts[type.c_str()]->addRow(simvars_label, widg);
            connect(widg, &Simvar::cellChanged, this, &SimvarMenu::cellChanged);
        } catch(std::out_of_range) {
            qDebug(("SimvarsMenu: intializer for "+string(type)+" not found").c_str());
        }
    }
    if(simvars_layouts["double"] != NULL) {
        ui->variablesLayout->addLayout(simvars_layouts["double"]);
    }
    if(simvars_layouts["int"] != NULL) {
        ui->variablesLayout->addLayout(simvars_layouts["int"]);
    }
    if(simvars_layouts["bool"] != NULL) {
        ui->variablesLayout->addLayout(simvars_layouts["bool"]);
    }
    QString celloptsLabel = static_cast<QLabel*>(simvars_layouts["cell_option"]->takeAt(0)->widget())->text();
    QWidget* cellopts = simvars_layouts["cell_option"]->takeAt(0)->widget();
    simvars_layouts["cell"]->addRow(celloptsLabel,cellopts);
    ui->variablesLayout->addLayout( simvars_layouts["cell"]);
    ui->filesLayout->addLayout(simvars_layouts["file"]);
    ui->filesLayout->addLayout(simvars_layouts["directory"]);
//tabs settup
    if(string(proto->type()) == "Grid Protocol") {
        this->grid = new GridSetupWidget(static_pointer_cast<GridProtocol>(this->proto));
        ui->tabs->addTab(grid, "Grid Setup");
        connect(grid, &GridSetupWidget::cellChanged, this, &SimvarMenu::cellChanged);
    }
//make menu match proto
    update_menu();
}

void SimvarMenu::update_menu() {
    for(auto& simv: this->simvars) {
        simv->update_ui();
    }
    emit updated();
}
void SimvarMenu::reset() {
    this->signalCellTypeChange = false;
    simvars.clear();
    qDeleteAll(this->children());
    ui->setupUi(this);
    createMenu();
    this->signalCellTypeChange = true;
}
void SimvarMenu::changeProto(shared_ptr<Protocol> proto) {
    this->proto = proto;
    this->reset();
}
void SimvarMenu::setWorkingDir(QDir&) {
    update_menu();
}
void SimvarMenu::changeCell(shared_ptr<Cell> cell) {
    if(cell != this->proto->cell()) {
        qWarning("SimvarMenu: Cell does not match protocol cell");
    }
    for(auto& simv: this->simvars) {
        simv->changeCell(cell);
    }
    update_menu();
}
void SimvarMenu::removeGrid() {
    this->grid->deleteLater();
    this->grid = 0;
}
