#include <QButtonGroup>
#include <QComboBox>
#include <QDate>
#include <QDir>
#include <QFileDialog>
#include <QFutureWatcher>
#include <QGridLayout>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QMainWindow>
#include <QProgressBar>
#include <QPushButton>
#include <QScrollArea>
#include <QSettings>
#include <QSpinBox>
#include <QSplitter>
#include <QStandardPaths>
#include <QtConcurrent>
#include <iterator>

#include "chooseprotowidget.h"
#include "dataoutputselectionwidget.h"
#include "graph.h"
#include "gridwrapper.h"
#include "longqtmainwindow.h"
#include "protocol.h"
#include "pvarmenu.h"
#include "runwidget.h"
#include "settingsIO.h"
#include "simfiles.h"
#include "simvarmenu.h"
#include "specialmenu.h"
#include "ui_longqtmainwindow.h"

using namespace std;
using namespace LongQt;

LongQtMainWindow::LongQtMainWindow(QString simvarFile, QWidget* parent)
    : QMainWindow(parent), ui(new Ui::LongQtMainWindow) {
  ui->setupUi(this);
  // create Widgets
  ChooseProtoWidget* choose = new ChooseProtoWidget(this);
  // read in simvars file if passed in
  if (simvarFile != "") {
    SettingsIO::getInstance()->readSettings(simvarFile, proto);
  } else {
    this->proto = choose->getCurrentProto();
  }
  SimFiles* sims = new SimFiles(proto, this);
  SpecialMenu* special = new SpecialMenu(this);
  GridWrapper* grid = new GridWrapper(proto, this);
  MvarMenu* mvars = new MvarMenu(proto, this);
  PvarMenu* pvars = new PvarMenu(proto, this);
  RunWidget* run = new RunWidget(proto, this);

  this->appendItem(
      choose, "Set Protocol and Cell",
      "Choose the type of protocol to follow in the simulation (e.g. fix "
      "the cell membrane voltage)");
  this->appendItem(sims, "Simulation Files",
                   "Change Directorys used by the simulation");
  this->appendItem(special, "", "");
  int special_pos = ui->menuStack->count() - 1;
  this->appendItem(grid, "Grid Setup", "");
  int grid_pos = ui->menuStack->count() - 1;
  this->setItemHidden(grid_pos, true);
  this->appendItem(pvars, "Set Model Parameters",
                   "Set model constants or have them randomly choosen");
  this->appendItem(
      mvars, "Data Output",
      "Select which model variables should be written to a file (e.g. Ca "
      "Concentration) Measure properties related to output variables (e.g. "
      "action "
      "potential duration, calcium amplitude)");
  this->appendItem(
      run, "Run Simulation",
      "Optionally write a description of the simulation and run the "
      "simulation");

  // menu
  connect(choose, &ChooseProtoWidget::protocolChanged, this,
          &LongQtMainWindow::protocolChanged);

  connect(this, &LongQtMainWindow::protocolChanged, choose,
          &ChooseProtoWidget::changeProto);
  connect(this, &LongQtMainWindow::protocolChanged, sims,
          &SimvarMenu::changeProto);
  connect(this, &LongQtMainWindow::protocolChanged, special,
          &SpecialMenu::changeProto);
  connect(this, &LongQtMainWindow::protocolChanged, grid,
          &GridWrapper::changeProto);
  connect(this, &LongQtMainWindow::protocolChanged, pvars,
          &PvarMenu::changeProto);
  connect(this, &LongQtMainWindow::protocolChanged, mvars,
          &MvarMenu::changeProto);
  connect(this, &LongQtMainWindow::protocolChanged, run, &RunWidget::setProto);
  connect(this, &LongQtMainWindow::protocolChanged, this,
          &LongQtMainWindow::changeProto);

  connect(choose, &ChooseProtoWidget::cellChanged, this,
          &LongQtMainWindow::cellChanged);
  connect(grid, &GridWrapper::cellChanged, this,
          &LongQtMainWindow::cellChanged);
  //  connect(sims, &SimvarMenu::cellChanged, this,
  //  &LongQtMainWindow::cellChanged);

  connect(this, &LongQtMainWindow::cellChanged, choose,
          &ChooseProtoWidget::changeCell);
  connect(this, &LongQtMainWindow::cellChanged, sims, &SimvarMenu::changeCell);
  connect(this, &LongQtMainWindow::cellChanged, special,
          &SpecialMenu::changeCell);
  connect(this, &LongQtMainWindow::cellChanged, mvars, &MvarMenu::reset);
  connect(this, &LongQtMainWindow::cellChanged, pvars, &PvarMenu::changeCell);
  connect(this, &LongQtMainWindow::cellChanged, mvars, &MvarMenu::changeCell);
  connect(run, &RunWidget::finished, this, &LongQtMainWindow::finished);

  connect(special, &SpecialMenu::noWidget,
          [this, special_pos]() { this->setItemHidden(special_pos, true); });
  connect(special, &SpecialMenu::nameChanged,
          [this, special_pos](QString name) {
            auto item = ui->menuList->item(special_pos);
            item->setText(name);
            item->setHidden(false);
          });

  connect(grid, &GridWrapper::widgetSet, [this, grid_pos](bool set) {
    auto item = ui->menuList->item(grid_pos);
    item->setHidden(!set);
  });

  special->changeProto(this->proto);
  this->setWindowTitle("LongQt");
  this->showMaximized();
  // connect buttons
  connect(ui->aboutButton, &QPushButton::clicked, [this]() {
    if (QMessageBox::RestoreDefaults ==
        QMessageBox::information(
            NULL, tr("LongQt Information"),
            tr(("Copyright© 2016 Thomas J. Hund\n"
                "License: GNU General Public License\n"
                "Email: thomas.hund@osumc.edu\n"
                "Version: " +
                QApplication::applicationVersion() +
                "\n"
                "Contributors:\n"
                "\tThomas Hund\n"
                "\tBirce Ela Onal\n"
                "\tDanielle Beckley\n"
                "\tDaniel Gratz\n")
                   .toStdString()
                   .c_str()),
            QMessageBox::Close | QMessageBox::RestoreDefaults,
            QMessageBox::Close)) {
      QSettings settings;
      settings.setValue("showHelp", true);
    }
  });
  connect(ui->menuList, &QListWidget::currentRowChanged, this,
          &LongQtMainWindow::list_click_aciton);
  connect(ui->nextButton, &QPushButton::clicked, this,
          &LongQtMainWindow::nextItem);
}

LongQtMainWindow::~LongQtMainWindow() {}

void LongQtMainWindow::insertItem(int pos, QWidget* widget, QString name,
                                  QString toolTip) {
  QScrollArea* scrollWraper = new QScrollArea();
  scrollWraper->setWidget(widget);
  scrollWraper->setWidgetResizable(true);

  ui->menuStack->insertWidget(pos, scrollWraper);
  ui->menuList->insertItem(pos, name);
  ui->menuList->item(ui->menuList->count() - 1)->setToolTip(toolTip);
}

void LongQtMainWindow::appendItem(QWidget* widget, QString name,
                                  QString toolTip) {
  this->insertItem(ui->menuList->count(), widget, name, toolTip);
}

void LongQtMainWindow::removeItem(int pos) {
  ui->menuStack->removeWidget(ui->menuStack->widget(pos));
  delete ui->menuList->takeItem(pos);
}

void LongQtMainWindow::setItemHidden(int pos, bool hidden) {
  ui->menuList->item(pos)->setHidden(hidden);
  int curIdx = ui->menuStack->currentIndex();
  if (pos == curIdx && hidden) {
    this->nextItem();
  }
}

void LongQtMainWindow::changeProto(shared_ptr<Protocol> proto) {
  this->proto = proto;
}
void LongQtMainWindow::list_click_aciton(int next_row) {
  ui->menuStack->setCurrentIndex(next_row);
  ui->menuList->setCurrentRow(next_row);
}
void LongQtMainWindow::nextItem() {
  int current_row = ui->menuStack->currentIndex();
  if (ui->menuStack->count() > current_row + 1) {
    ui->menuStack->setCurrentIndex(current_row + 1);
    ui->menuList->setCurrentRow(current_row + 1);
    if (ui->menuList->item(current_row + 1)->isHidden()) {
      this->nextItem();
    }
  }
}
void LongQtMainWindow::finished() {
  QMessageBox::information(0, "Folder",
                           ("Simulation is finished!\n The folder is named: " +
                               proto->datadir.string()).c_str());
  try {
    ui->menuStack->addWidget(new Grapher(QString(proto->datadir.string().c_str()), this));
    ui->menuList->addItem("Graph " +
                          QFileInfo(proto->datadir.string().c_str()).baseName());
  } catch (std::runtime_error& e) {
    Logger::getInstance()->write("HearCellSim: data files not readable {}",
                                 e.what());
  }
  proto->setDataDir();
  proto->cellStateDir = proto->datadir;
}
