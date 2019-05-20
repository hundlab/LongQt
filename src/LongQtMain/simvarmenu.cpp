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
  this->createMenu();
}

SimvarMenu::~SimvarMenu() { delete ui; }

void SimvarMenu::setNames(std::list<std::list<std::string> > names) {
  this->names = names;
  this->reset();
}

void SimvarMenu::createMenu() {
  QMap<QString, QFormLayout*> simvars_layouts;
  map<string, SimvarInitializer> initializers = {
      {"double",
       [](shared_ptr<Protocol> proto, string name) {
         auto widg = new QDoubleSpinBox;
         return std::make_pair(widg, new SimvDouble(proto, name, widg));
       }},
      {"int",
       [](shared_ptr<Protocol> proto, string name) {
         auto widg = new QSpinBox;
         return std::make_pair(widg, new SimvInt(proto, name, widg));
       }},
      {"bool",
       [](shared_ptr<Protocol> proto, string name) {
         auto widg = new QCheckBox;
         return std::make_pair(widg, new SimvBool(proto, name, widg));
       }},
      {"file",
       [](shared_ptr<Protocol> proto, string name) {
         auto widg = new QLineEdit;
         return std::make_pair(widg, new SimvFile(proto, name, widg));
       }},
      {"directory",
       [](shared_ptr<Protocol> proto, string name) {
         auto widg = new QWidget;
         return std::make_pair(widg, new SimvDir(proto, name, widg));
       }},
      {"cell",
       [this](shared_ptr<Protocol> proto, string name) {
         auto widg = new QComboBox;
         auto simv = new SimvCell(proto, name, widg);
         connect(simv, &SimvCell::cellChanged, this, &SimvarMenu::cellChanged);
         return std::make_pair(widg, simv);
       }},
      {"cell_option", [](shared_ptr<Protocol> proto, string name) {
         auto widg = new QGroupBox;
         return std::make_pair(widg, new SimvCellOpts(proto, name, widg));
       }}};
  // do all the work for simvars setup
  int col = 0;
  int row = 0;
  for (auto namesList : this->names) {
    for (auto parname : namesList) {
      auto name = parname.c_str();
      auto type = this->proto->parsType(name);
      try {
        QWidget* widg;
        Simvar* simvar;
        std::tie(widg, simvar) = initializers.at(type)(proto, name);
        QLabel* label = new QLabel;
        widg->setObjectName(name);
        simvar->setupLabel(label);
        simvars.insert(name, simvar);
        ui->itemsLayout->addWidget(label, row, col);
        ui->itemsLayout->addWidget(widg, row, col + 1);
        ++row;
      } catch (std::out_of_range) {
        Logger::getInstance()->write(
            "SimvarsMenu: intializer for {} not found ", type);
      }
    }
    col += 2;
    row = 0;
  }
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
  simvars.clear();
  qDeleteAll(this->children());
  ui->setupUi(this);
  createMenu();
}
void SimvarMenu::changeProto(shared_ptr<Protocol> proto) {
  this->proto = proto;
  this->reset();
}
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
