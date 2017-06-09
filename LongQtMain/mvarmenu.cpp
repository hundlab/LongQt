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
#include "mvarmenu.h"

/*#################################
    begin mvarMenu class
###################################*/
mvarMenu::mvarMenu(Protocol* initial_proto, QWidget *parent):QWidget(parent)  {
//setup class variables
    proto = initial_proto;

    this->dvarsDescriptions = GuiUtils::concatMaps(
                GuiUtils::readMap(":/hoverText/dvarsDescriptions.txt"),
                ", ",GuiUtils::readMap(":/hoverText/dvarsUnits.txt"));
    this->measDescriptions = GuiUtils::readMap(":/hoverText/measDescriptions.txt");
    this->createMenu();
}

void mvarMenu::createMenu()  {
//setup useful constants and aliases
    unsigned int row_len = 6;
    Measure temp = Measure();
    QString end_op = "Next";
//initialize layouts and signal maps
    QGridLayout* main_layout = new QGridLayout(this);
    QGridLayout* central_layout = new QGridLayout;
//initialize buttons &lables
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
    main_layout->addLayout(central_layout, 1,0, row_len, row_len); 
    setLayout(main_layout); 
    setWindowTitle(tr("Output Variables Menu"));
//connect buttons   
    connect(vars_view, &QListWidget::currentRowChanged, this, &mvarMenu::switch_var);
    connect(addto_vars_options, &QComboBox::currentTextChanged, this, &mvarMenu::checkMeasOpts);
    connect(addto_vars_list_button, &QPushButton::clicked, this, &mvarMenu::addto_vars_list);
    connect(removefr_vars_list_button, &QPushButton::clicked, this, &mvarMenu::removefr_vars_list);
    connect(addto_meas_list_button, &QPushButton::clicked, this, &mvarMenu::addto_meas_list);
    connect(removefr_meas_list_button, &QPushButton::clicked, this, &mvarMenu::removefr_meas_list);
    connect(percrepol_spinbox, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),
        [this] (int val) {
            auto measures = this->proto->Measures;
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
        QList<QListWidgetItem*> vars_item = vars_view->findItems(it->second.varname.c_str()
            ,Qt::MatchExactly);
        if(vars_item.empty()) {
            QListWidgetItem* to_add = new QListWidgetItem(it->second.varname.c_str());
            to_add->setToolTip(dvarsDescriptions[it->second.varname.c_str()]);
            vars_view->addItem(to_add);
            row = vars_view->row(vars_view->findItems(it->second.varname.c_str(),
             Qt::MatchExactly).first());
        }
    }

    meas_view->clear();
    vars_view->setCurrentRow(row);
    if(vars_view->currentRow() >= 0) {
        auto selection = proto->Measures.at(vars_view->currentItem()->text().toStdString())
                .Selection;
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
        proto->removeFromMeasureSelection(vars_view->currentItem()->text().toStdString(),
                                          taken_meas->text().toStdString());
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
        for(set<string>::iterator set_it = this->measure_options.begin();
            set_it != this->measure_options.end(); set_it++) {

            addto_meas_options->addItem(set_it->c_str());
            addto_meas_options->setItemData(i,measDescriptions[set_it->c_str()],Qt::ToolTipRole);
            i++;
        }
    } else {
        addto_meas_options->addItem("peak");
        addto_meas_options->setItemData(0,measDescriptions["peak"],Qt::ToolTipRole);
    }
};


