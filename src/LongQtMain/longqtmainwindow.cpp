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
#include "dvarmenu.h"
#include "graph.h"
#include "heartcellsim.h"
#include "mvarmenu.h"
#include "protocol.h"
#include "pvarmenu.h"
#include "runwidget.h"
#include "settingsIO.h"
#include "simvarmenu.h"
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
  SimvarMenu* sims = new SimvarMenu(proto, this);
  MvarMenu* mvars = new MvarMenu(proto, this);
  PvarMenu* pvars = new PvarMenu(proto, this);
  RunWidget* run = new RunWidget(proto, this);

  this->appendItem(
      choose, "Set Protocol",
      "Choose the type of protocol to follow in the simulation (e.g. fix "
      "the cell membrane voltage)");
  this->appendItem(
      sims, "Set Sim. Parameters",
      "Change model parameters (e.g. change an ion channel conductance)");
  this->appendItem(pvars, "Set Model Parameters",
                   "Set model constants or have them randomly choosen");
  this->appendItem(
      mvars, "Select Output",
      "Select which model variables should be written to a file (e.g. Ca "
      "Concentration) Measure properties related to output variables (e.g. "
      "action "
      "potential duration, calcium amplitude)");
  this->appendItem(
      run, "Run Simulation",
      "Optionally write a description of the simulation and run the "
      "simulation");

  // menu
  connect(choose, &ChooseProtoWidget::protocolChanged, sims,
          &SimvarMenu::changeProto);
  connect(choose, &ChooseProtoWidget::protocolChanged, pvars,
          &PvarMenu::changeProto);
  connect(choose, &ChooseProtoWidget::protocolChanged, mvars,
          &MvarMenu::changeProto);
  connect(choose, &ChooseProtoWidget::protocolChanged, run,
          &RunWidget::setProto);
  connect(choose, &ChooseProtoWidget::protocolChanged, this,
          &LongQtMainWindow::changeProto);

  connect(choose, &ChooseProtoWidget::cellChanged, this,
          &LongQtMainWindow::cellChanged);
  connect(this, &LongQtMainWindow::cellChanged, choose,
          &ChooseProtoWidget::changeCell);
  connect(sims, &SimvarMenu::cellChanged, this, &LongQtMainWindow::cellChanged);
  connect(this, &LongQtMainWindow::cellChanged, sims, &SimvarMenu::changeCell);
  connect(this, &LongQtMainWindow::cellChanged, mvars, &MvarMenu::reset);
  connect(this, &LongQtMainWindow::cellChanged, pvars, &PvarMenu::changeCell);
  connect(this, &LongQtMainWindow::cellChanged, mvars, &MvarMenu::changeCell);
  connect(run, SIGNAL(canceled()), this, SLOT(canceled()));
  connect(run, SIGNAL(finished()), this, SLOT(finished()));
  connect(run, SIGNAL(running()), this, SLOT(running()));

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
  connect(ui->menuList, SIGNAL(currentRowChanged(int)), this,
          SLOT(list_click_aciton(int)));
  connect(ui->nextButton, SIGNAL(clicked()), this, SLOT(next_button_aciton()));

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

void LongQtMainWindow::changeProto(shared_ptr<Protocol> proto) {
  this->proto = proto;
}
void LongQtMainWindow::list_click_aciton(int next_row) {
  ui->menuStack->setCurrentIndex(next_row);
  ui->menuList->setCurrentRow(next_row);
}
void LongQtMainWindow::next_button_aciton() {
  int current_row = ui->menuStack->currentIndex();
  if (ui->menuStack->count() > current_row + 1) {
    ui->menuStack->setCurrentIndex(current_row + 1);
    ui->menuList->setCurrentRow(current_row + 1);
  }
}
void LongQtMainWindow::finished() {
  QMessageBox::information(0, "Folder",
                           "Simulation is finished!\n The folder is named: " +
                               proto->datadir.absolutePath());
  try {
    ui->menuStack->addWidget(new Grapher(proto->datadir, this));
    ui->menuList->addItem("Graph " +
                          QFileInfo(proto->datadir.absolutePath()).baseName());
  } catch (std::runtime_error& e) {
    Logger::getInstance()->write("HearCellSim: data files not readable {}",
                                 e.what());
  }
  proto->setDataDir();
  proto->cellStateDir = proto->datadir;
}
