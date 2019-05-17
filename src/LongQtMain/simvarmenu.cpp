#include <gridsetupwidget.h>
#include <logger.h>
#include <voltageClampProtocol.h>
#include <QCheckBox>
#include <QCloseEvent>
#include <QComboBox>
#include <QDate>
#include <QDoubleSpinBox>
#include <QFile>
#include <QFileDialog>
#include <QFormLayout>
#include <QGridLayout>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QLabel>
#include <QList>
#include <QListWidget>
#include <QMessageBox>
#include <QPushButton>
#include <QRadioButton>
#include <QString>
#include <QTabWidget>
#include <QTableWidget>
#include <QToolTip>
#include <QWidget>
#include <limits>

#include "guiUtils.h"
#include "simvarmenu.h"
#include "simvbool.h"
#include "simvcell.h"
#include "simvcellopts.h"
#include "simvdir.h"
#include "simvdouble.h"
#include "simvfile.h"
#include "simvint.h"
#include "ui_simvarmenu.h"
using namespace std;
using namespace LongQt;

SimvarMenu::SimvarMenu(shared_ptr<Protocol> initial_proto, QWidget* parent)
    : QWidget(parent), ui(new Ui::SimvarMenu) {
  ui->setupUi(this);
  // setup class variables
  proto = initial_proto;
  descriptions = GuiUtils::readDesc(":/hoverText/simvarsDesc.json");
  this->createMenu();
}

SimvarMenu::~SimvarMenu() {
  /*if (this->grid) {
    this->grid->deleteLater();
    this->grid = 0;
  }
  if (this->voltageClamp) {
    this->voltageClamp->deleteLater();
    this->voltageClamp = 0;
  }*/
  delete ui;
}

void SimvarMenu::createMenu() {
  QMap<QString, QFormLayout*> simvars_layouts;
  map<string, SimvarInitializer> initializers = {
      {"double",
       [](shared_ptr<Protocol> proto, string name) {
         auto widg = new QDoubleSpinBox;
         return std::make_pair(widg,new SimvDouble(proto, name, widg));
       }},
      {"int",
       [](shared_ptr<Protocol> proto, string name) {
         auto widg = new QSpinBox;
         return  std::make_pair(widg,new SimvInt(proto, name, widg));
       }},
      {"bool", [](shared_ptr<Protocol> proto,
                  string name) {
           auto widg = new QCheckBox;
           return  std::make_pair(widg, new SimvBool(proto, name, widg)); }},
      {"file", [](shared_ptr<Protocol> proto,
                  string name) {
           auto widg = new QLineEdit;
           return  std::make_pair(widg, new SimvFile(proto, name, widg)); }},
      {"directory", [](shared_ptr<Protocol> proto,
                       string name) {
           auto widg = new QWidget;
           return  std::make_pair(widg, new SimvDir(proto, name, widg)); }}
/*      {"cell", [this](shared_ptr<Protocol> proto,
                  string name) { auto simv = new SimvCell(proto, name);
     connect(simv, &Simvar::cellChanged, this, &SimvarMenu::cellChanged);
    return simv;}},
      {"cell_option", [](shared_ptr<Protocol> proto, string name) {
         return new SimvCellOpts(proto, name);
       }}*/};
  // do all the work for simvars setup
  for (auto par : proto->parsList()) {
    auto name = par.first.c_str();
    auto type = par.second.c_str();
    if (simvars_layouts.find(type) == simvars_layouts.end()) {
      simvars_layouts.insert(type, new QFormLayout());
    }
    try {
      QWidget* widg;
      Simvar* simvar;
      std::tie(widg, simvar) = initializers.at(type)(proto, name);
      QLabel* simvars_label = new QLabel;
      simvar->setupLabel(simvars_label);
      simvars.insert(name, simvar);
      simvars_layouts[type]->addRow(simvars_label, static_cast<QWidget*>(widg));
      widg->setObjectName(name);
    } catch (std::out_of_range) {
      //      Logger::getInstance()->write("SimvarsMenu: intializer for {} not
      //      found",
      //                                   type);
    }
  }
  if (simvars_layouts["double"] != NULL) {
    ui->variablesLayout->addLayout(simvars_layouts["double"]);
  }
  if (simvars_layouts["int"] != NULL) {
    ui->variablesLayout->addLayout(simvars_layouts["int"]);
  }
  if (simvars_layouts["bool"] != NULL) {
    ui->variablesLayout->addLayout(simvars_layouts["bool"]);
  }
  //  QString celloptsLabel =
  //      static_cast<QLabel*>(simvars_layouts["cell_option"]->takeAt(0)->widget())
  //          ->text();
  //  QWidget* cellopts = simvars_layouts["cell_option"]->takeAt(0)->widget();
  //  simvars_layouts["cell"]->addRow(celloptsLabel, cellopts);
  //  ui->variablesLayout->addLayout(simvars_layouts["cell"]);
  ui->filesLayout->addLayout(simvars_layouts["file"]);
  ui->filesLayout->addLayout(simvars_layouts["directory"]);
  // tabs settup
  /*  if (string(proto->type()) == "Grid Protocol") {
      this->grid =
          new GridSetupWidget(static_pointer_cast<GridProtocol>(this->proto));
      ui->tabs->addTab(grid, "Grid Setup");
      connect(grid, &GridSetupWidget::cellChanged, this,
              &SimvarMenu::cellChanged);
    } else if (string(proto->type()) == "Voltage Clamp Protocol") {
      this->voltageClamp = new VoltageClampSetupWidget(
          static_pointer_cast<VoltageClamp>(this->proto), this);
      ui->tabs->addTab(voltageClamp, "Voltage Clamp Setup");
      //        connect(voltageClamp, &GridSetupWidget::cellChanged, this,
      //        &SimvarMenu::cellChanged);
    }*/
  // make menu match proto
  update_menu();
}

void SimvarMenu::update_menu() {
  for (auto& simv : this->simvars) {
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
void SimvarMenu::setWorkingDir(QDir&) { update_menu(); }
void SimvarMenu::changeCell(shared_ptr<Cell> cell) {
  if (cell != this->proto->cell()) {
    Logger::getInstance()->write(
        "SimvarMenu: Cell does not match protocol cell");
  }
  for (auto& simv : this->simvars) {
    simv->changeCell(cell);
  }
  update_menu();
}
void SimvarMenu::removeGrid() {
/*  if (this->grid) {
    this->grid->deleteLater();
    this->grid = 0;
  }*/
}
