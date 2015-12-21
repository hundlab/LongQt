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

#include "varmenu.h"
#include "proto.h"



simvarMenu::simvarMenu(QWidget *parent, Protocol initial_proto) {
    unsigned int i, row_len = 6;
    proto = initial_proto;
    this->parent = parent;
    map<string,double*> pars = proto.pars;
    map<string,double*>::iterator it;
    simvar_map = new QSignalMapper(this);
    QPushButton* get_vars = new QPushButton("Read File");
    QPushButton* set_vars = new QPushButton("Write File");
    QGridLayout* main_layout = new QGridLayout;

   
//    QCheckBox readflag = new QCheckBox("Read in variable files", this);
    simvars = (QDoubleSpinBox**)malloc(pars.size()*sizeof(QDoubleSpinBox*));
    simvar_names = (QLabel**)malloc(pars.size()*sizeof(QLabel*));

    for(it = pars.begin(), i = 0; it!=pars.end(); it++,i++) {
        simvars[i] = new QDoubleSpinBox(this);
        simvar_names[i] = new QLabel(*(new QString((it->first).c_str())),this);
        main_layout->addWidget(simvar_names[i],(2*i)/row_len,(2*i)%row_len);
        main_layout->addWidget(simvars[i],(2*i+1)/row_len,(2*i+1)%row_len);
        connect(simvars[i], SIGNAL(editingFinished()),simvar_map, SLOT(map()));
        simvar_map->setMapping(simvars[i],i);
    }
    
    connect(simvar_map, SIGNAL(mapped(int)), this, SLOT(update_pvars(int)));
    connect(get_vars, SIGNAL(clicked()), this, SLOT(read_simvars())); 
    connect(set_vars, SIGNAL(clicked()), this, SLOT(write_simvars())); 
    main_layout->addWidget(get_vars, (2*i)/row_len +1,row_len -1);
    main_layout->addWidget(set_vars, (2*i)/row_len +1,row_len -2);
    setLayout(main_layout); 
    setWindowTitle(tr("Simulation Variables Menu"));

    update_menu();
    
}

simvarMenu::~simvarMenu(){}

bool simvarMenu::read_simvars(){

    bool ret = false;
    QString fileName = QFileDialog::getOpenFileName(this);
    if (!fileName.isEmpty()){
        proto.simvarfile = fileName.toStdString();
    ret = (bool)proto.readpars(proto.pars, proto.simvarfile);
    }
    update_menu();
    return ret;
}

bool simvarMenu::write_simvars(){
    
    bool ret = false;
    QString fileName = QFileDialog::getOpenFileName(this);
    if (!fileName.isEmpty()){
        proto.simvarfile = fileName.toStdString();
    ret = (bool)proto.writepars(proto.pars, proto.simvarfile);
    }
    return ret;

}

void simvarMenu::update_menu() {
    map<string,double*>::iterator it;
    unsigned int i; 
    for(it = proto.pars.begin(), i = 0; it != proto.pars.end(); it++, i++)     {  
        simvars[i]->setValue(*(it->second));
    }
}

void simvarMenu::update_pvars(int id){
    *(proto.pars[simvar_names[id]->text().toStdString()]) = simvars[id]->value();
}
