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

	proto->datadir = (QStandardPaths::standardLocations(QStandardPaths::DocumentsLocation).first() + "/data" + date_time).toStdString();
	proto->cellStateDir = proto->datadir;
	simvarMenu* sims = new simvarMenu(proto, this);
	dvarMenu* dvars = new dvarMenu(proto->cell, this);
	mvarMenu* mvars =  new mvarMenu(proto, this);
	PvarMenu* pvars =  new PvarMenu(proto, this);
	RunWidget* run = new RunWidget(proto,QDir(proto->datadir.c_str()));
	//add items menu_list
	menu_list.append(choose);
	menu_list.append(sims);
	menu_list.append(pvars);
	menu_list.append(dvars);
	menu_list.append(mvars);
	menu_list.append(run);

	connect(choose, SIGNAL(protocolChanged(Protocol*)), sims, SLOT(changeProto(Protocol*)));
	connect(choose, SIGNAL(protocolChanged(Protocol*)), pvars, SLOT(changeProto(Protocol*)));
	connect(choose, SIGNAL(protocolChanged(Protocol*)), mvars, SLOT(changeProto(Protocol*)));
	connect(choose, SIGNAL(protocolChanged(Protocol*)), run, SLOT(setProto(Protocol*)));
	connect(choose, SIGNAL(protocolChanged(Protocol*)), this, SLOT(changeProto(Protocol*)));

	connect(choose, SIGNAL(cellChanged(Cell*)), this, SIGNAL(cellChanged(Cell*)));
	connect(this, SIGNAL(cellChanged(Cell*)), choose, SLOT(changeCell(Cell*)));
	connect(sims, SIGNAL(cellChanged(Cell*)), this, SIGNAL(cellChanged(Cell*)));
	connect(this, SIGNAL(working_dir_changed(QDir&)), run, SLOT(setWorkingDir(QDir&)));
	connect(this, SIGNAL(cellChanged(Cell*)), sims, SLOT(changeCell(Cell*)));
	connect(this, &Simulation::cellChanged,dvars,&dvarMenu::changeCell);
	connect(this, SIGNAL(cellChanged(Cell*)), mvars, SLOT(reset()));
	connect(this, &Simulation::cellChanged,pvars,&PvarMenu::changeCell);
	connect(run, SIGNAL(canceled()), this, SLOT(canceled()));
	//    connect(run, SIGNAL(finished()), choose, SLOT(resetProto()));
	connect(run, SIGNAL(finished()), this, SLOT(finished()));
	connect(run, SIGNAL(running()), this, SLOT(running()));
	connect(run, static_cast<void(RunWidget::*)()>(&RunWidget::running),
			[this] () {
			SettingsIO::getInstance()->writeSettings(this->proto,(this->proto->datadir+"/"+this->proto->simvarfile).c_str());
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
							"License: Revised BSD License\n"
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
		SettingsIO::getInstance()->readSettings(proto,simvarFile);
	}
};
Simulation::~Simulation(){};
void Simulation::changeProto(Protocol* proto) {
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
	date_time = QDate::currentDate().toString("MMddyy") + "-" + QTime::currentTime().toString("hhmm");
//	QDir working_dir = (QStandardPaths::standardLocations(QStandardPaths::DocumentsLocation).first() + "/data" + date_time);

}
void Simulation::canceled() {
	qDebug()<<"canceled!";
	QMessageBox::critical(this,"Cancel","Simulation canceled!");
	date_time = QDate::currentDate().toString("MMddyy") + "-" + QTime::currentTime().toString("hhmm");
	QDir working_dir = (QStandardPaths::standardLocations(QStandardPaths::DocumentsLocation).first() + "/data" + date_time);
	for(int i = 1; working_dir.exists(); i++) {
		working_dir = (QStandardPaths::standardLocations(QStandardPaths::DocumentsLocation).first() + "/data" + date_time + "_" + QString::number(i));
	}
	proto->datadir = working_dir.absolutePath().toStdString();
	proto->cellStateDir = proto->datadir;
	emit working_dir_changed(working_dir);
	cancel_button->hide();
	next_button->show();
}
void Simulation::finished() {
	QMessageBox::information(0,"Folder", "Simulation is finished!\n The folder is named: " + QString(proto->datadir.c_str()));
	try {
		menu_list.append(new Grapher(QDir(proto->datadir.c_str()), this));        
		menu->addWidget(menu_list.last());
		menu_options->addItem("Graph " + QFileInfo(proto->datadir.c_str()).baseName());
	} catch(BadFile& e) {
		qCritical() << e.what() << ": " << "data files not readable";
	}
	date_time = QDate::currentDate().toString("MMddyy") + "-" + QTime::currentTime().toString("hhmm");
	QDir working_dir = (QStandardPaths::standardLocations(QStandardPaths::DocumentsLocation).first() + "/data" + date_time);
	for(int i = 1; working_dir.exists(); i++) {
		working_dir = (QStandardPaths::standardLocations(QStandardPaths::DocumentsLocation).first() + "/data" + date_time + "_" + QString::number(i));
	}
	proto->datadir = working_dir.absolutePath().toStdString();
	proto->cellStateDir = proto->datadir;
	emit working_dir_changed(working_dir);
	cancel_button->hide();
	next_button->show();
}
void Simulation::running() {
	next_button->hide();
	cancel_button->show();
}
