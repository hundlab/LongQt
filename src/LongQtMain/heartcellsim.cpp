#include <QMainWindow>
#include <QGridLayout>
#include <QButtonGroup>
#include <QHBoxLayout>
#include <QPushButton>
#include <QComboBox>
#include <QSpinBox>
#include <QScrollArea>
#include <QLabel>
#include <QFutureWatcher>
#include <QtConcurrent>
#include <QDir>
#include <QDate>
#include <QGroupBox>
#include <iterator>
#include <QProgressBar>
#include <QFileDialog>
#include <QStandardPaths>
#include <QSplitter>
#include <QSettings>

#include "protocol.h"
#include "heartcellsim.h"
#include "simvarmenu.h"
#include "dvarmenu.h"
#include "mvarmenu.h"
#include "pvarmenu.h"
#include "graph.h"
#include "runwidget.h"
#include "chooseprotowidget.h"
#include "settingsIO.h"

using namespace std;
using namespace LongQt;

Simulation::Simulation(QString simvarFile, QWidget* parent){
	//setup class variables
	this->parent = parent;
	date_time = QDate::currentDate().toString("MMddyy") + "-" + QTime::currentTime().toString("hhmm");
	//    bool was_grid = false;
	//create layouts
	main_layout = new QGridLayout(this);
	//organizational widgets
	menu = new QStackedWidget();
	menu_options = new QListWidget();
	//create Widgets
	QSplitter* main_splitter = new QSplitter();
	about_button = new QPushButton(QIcon::fromTheme("help-about"),"About");
	next_button = new QPushButton("Next");
	cancel_button = new QPushButton("Cancel");
	ChooseProtoWidget* choose = new ChooseProtoWidget(this);
	this->proto = choose->getCurrentProto();

    SimvarMenu* sims = new SimvarMenu(proto, this);
	dvarMenu* dvars = new dvarMenu(proto->cell(), this);
	MvarMenu* mvars =  new MvarMenu(proto, this);
	PvarMenu* pvars =  new PvarMenu(proto, this);
	RunWidget* run = new RunWidget(proto,proto->datadir);
	//add items menu_list
	menu_list.append(choose);
	menu_list.append(sims);
	menu_list.append(pvars);
	menu_list.append(dvars);
	menu_list.append(mvars);
	menu_list.append(run);

    connect(choose, &ChooseProtoWidget::protocolChanged, sims, &SimvarMenu::changeProto);
    connect(choose, &ChooseProtoWidget::protocolChanged, pvars, &PvarMenu::changeProto);
    connect(choose, &ChooseProtoWidget::protocolChanged, mvars, &MvarMenu::changeProto);
    connect(choose, &ChooseProtoWidget::protocolChanged, run, &RunWidget::setProto);
    connect(choose, &ChooseProtoWidget::protocolChanged, this, &Simulation::changeProto);

    connect(choose, &ChooseProtoWidget::cellChanged, this, &Simulation::cellChanged);
    connect(this, &Simulation::cellChanged, choose, &ChooseProtoWidget::changeCell);
    connect(sims, &SimvarMenu::cellChanged, this, &Simulation::cellChanged);
    connect(this, &Simulation::working_dir_changed, run, &RunWidget::setWorkingDir);
    connect(this, &Simulation::cellChanged, sims, &SimvarMenu::changeCell);
	connect(this, &Simulation::cellChanged,dvars,&dvarMenu::changeCell);
    connect(this, &Simulation::cellChanged, mvars, &MvarMenu::reset);
	connect(this, &Simulation::cellChanged,pvars,&PvarMenu::changeCell);
	connect(this, &Simulation::cellChanged,mvars,&MvarMenu::changeCell);
	connect(run, SIGNAL(canceled()), this, SLOT(canceled()));
	//    connect(run, SIGNAL(finished()), choose, SLOT(resetProto()));
	connect(run, SIGNAL(finished()), this, SLOT(finished()));
	connect(run, SIGNAL(running()), this, SLOT(running()));
	connect(run, static_cast<void(RunWidget::*)()>(&RunWidget::running),
			[this] () {
            SettingsIO::getInstance()->writeSettings((this->proto->getDataDir()+"/"+this->proto->simvarfile).c_str(),this->proto);
			});
	connect(cancel_button, SIGNAL(clicked()),run, SLOT(cancel()));
	//set button/combo box inital values
	cancel_button->hide();
	//menu
	menu_options->addItem("Set Protocol");
	menu_options->item(menu_options->count()-1)->setToolTip("Choose the type of protocol to follow in the simulation (e.g. fix the cell membrane voltage)");
	menu_options->addItem("Set Sim. Parameters");
	menu_options->item(menu_options->count()-1)->setToolTip("Change model parameters (e.g. change an ion channel conductance)");
	menu_options->addItem("Set Model Parameters");
	menu_options->item(menu_options->count()-1)->setToolTip("Set model constants or have them randomly choosen");
	menu_options->addItem("Select Output");
	menu_options->item(menu_options->count()-1)->setToolTip("Select which model variables should be written to a file (e.g. Ca Concentration)");
	menu_options->addItem("Select Measured Props");
	menu_options->item(menu_options->count()-1)->setToolTip("Measure properties related to output variables (e.g. action potential duration, calcium amplitude)");
	menu_options->addItem("Run Simulation");
	menu_options->item(menu_options->count()-1)->setToolTip("Optionally write a description of the simulation and run the simulation");
	for(auto it = menu_list.begin(); it != menu_list.end(); it++) {
		QScrollArea* scrollWraper = new QScrollArea();
		scrollWraper->setWidget(*it);
		scrollWraper->setWidgetResizable(true);
		menu->addWidget(scrollWraper);
	}
	//main_splitter
	main_splitter->addWidget(menu_options);
	main_splitter->addWidget(menu);
	QList<int> tempList = QList<int>();
	tempList << 10 << 200;
	main_splitter->setSizes(tempList);
	//main_layout
	main_layout->addWidget(main_splitter, 0,0,-1,1);
	main_layout->addWidget(about_button, 0, 2);
	main_layout->addWidget(next_button, 2, 2);
	main_layout->addWidget(cancel_button, 2, 2);

	this->setWindowTitle("LongQt");
	this->showMaximized();
	//connect buttons
	connect(about_button, &QPushButton::clicked, [this] () {
			if(QMessageBox::RestoreDefaults == 
					QMessageBox::information(NULL,tr("LongQt Information"),
						tr(("Copyright© 2016 Thomas J. Hund\n"
                            "License: GNU General Public License\n"
							"Email: thomas.hund@osumc.edu\n"
							"Version: "+QApplication::applicationVersion()+"\n"
							"Contributers:\n"
							"\tThomas Hund\n"
							"\tBirce Ela Onal\n"
							"\tDanielle Beckley\n"
							"\tDaniel Gratz\n").toStdString().c_str())
						,QMessageBox::Close|QMessageBox::RestoreDefaults,QMessageBox::Close)) {
			QSettings settings;
			settings.setValue("showHelp",true);
			}
			});
	connect(menu_options, SIGNAL(currentRowChanged(int)), this, SLOT(list_click_aciton(int)));
	connect(next_button, SIGNAL(clicked()), this, SLOT(next_button_aciton()));
	//read in simvars file if passed in
	if(simvarFile != "") {
        SettingsIO::getInstance()->readSettings(simvarFile,proto);
	}
}
Simulation::~Simulation(){}

void Simulation::changeProto(shared_ptr<Protocol> proto) {
	this->proto = proto;
}
void Simulation::list_click_aciton (int next_row) {
	menu->setCurrentIndex(next_row);
	menu_options->setCurrentRow(next_row);
}
void Simulation::next_button_aciton () {
	int current_row = menu->currentIndex();
	if(menu->count() > current_row +1) {
		menu->setCurrentIndex(current_row +1);
		menu_options->setCurrentRow(current_row +1);
	}
}
void Simulation::canceled() {
	qDebug()<<"canceled!";
	QMessageBox::critical(this,"Cancel","Simulation canceled!");
    proto->setDataDir();
    proto->cellStateDir = proto->datadir;
	emit working_dir_changed(proto->datadir);
	cancel_button->hide();
	next_button->show();
}
void Simulation::finished() {
	QMessageBox::information(0,"Folder", "Simulation is finished!\n The folder is named: " + proto->datadir.absolutePath());
	try {
		menu_list.append(new Grapher(proto->datadir, this));
		menu->addWidget(menu_list.last());
		menu_options->addItem("Graph " + QFileInfo(proto->datadir.absolutePath()).baseName());
	} catch(BadFile& e) {
		qCritical() << e.what() << ": " << "data files not readable";
	}
    proto->setDataDir();
	proto->cellStateDir = proto->datadir;
	emit working_dir_changed(proto->datadir);
	cancel_button->hide();
	next_button->show();
}
void Simulation::running() {
	next_button->hide();
	cancel_button->show();
}
