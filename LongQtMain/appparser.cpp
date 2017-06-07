#include "heartcellsim.h"
#include "CLISimulation.h"
#include "appparser.h"

#include <QApplication>
#include <QMessageBox>
#include <QSettings>

AppParser::AppParser(QCoreApplication* app):
	GUIOption("no-gui",
			QCoreApplication::translate("main", "Start LongQt in CLI mode")),
	licenseOption(QStringList() << "l" << "license",
			QCoreApplication::translate("main", "Print the license and exit")),
	simvarsFileOption("simvars",
				QCoreApplication::translate("main",
					"Specify the file for setting simulation parameters e.g. number "
					"of trials"),
				QCoreApplication::translate("main","simvars.txt"))
{
	this->app = app;
	parser.setApplicationDescription(
			"Copyright© 2016 Thomas J. Hund\n"
			"License: Revised BSD License\n"
			"Email: thomas.hund@osumc.edu\n"
			"Version: "+QApplication::applicationVersion()+"\n"
			"Contributers:\n"
			"\tThomas Hund\n"
			"\tBirce Ela Onal\n"
			"\tDanielle Beckley\n"
			"\tDaniel Gratz\n");
	parser.addHelpOption();
	parser.addVersionOption();
	parser.addOption(GUIOption);
	parser.addOption(licenseOption);
	parser.addOption(simvarsFileOption);
}

void AppParser::process() {
	parser.process(*app);
	bool license = parser.isSet(licenseOption);
	
	this->simvarsFile = parser.value(simvarsFileOption);

	if(license) {
		this->showLicense();
		exit(0);
	}
}

void AppParser::showLicense() {
	QFile* licenseFile = new QFile(":/LICENSE");
	licenseFile->open(QIODevice::ReadOnly|QIODevice::Text);
	if(licenseFile->isOpen()) {
		QTextStream licenseStream(licenseFile);
		cout << licenseStream.readAll().toStdString();
	}
}

void AppParser::start() {
	bool startCLI = parser.isSet(GUIOption);

	if(!startCLI) {
		Simulation* window = new Simulation(simvarsFile);
		window->show();
		QSettings settings;
		if(settings.value("showHelp",true).toBool() &&QMessageBox::Discard == QMessageBox::information(0,"Welcome!", "LongQt is a program for modeling cardiac potentials. As you go through this program keep a few things in mind. First, if you would like more information about something hover your mouse above its name and information will pop up. Second, default values have been provided for all options so if you don't know what an option does it's ok to simply skip it. And finally have fun!\n If you would like to re-enable this text after discarding it use the Restore Defaults button in the About dialog.", QMessageBox::Ok|QMessageBox::Discard,QMessageBox::Ok)) {
			settings.setValue("showHelp",false);
		}
	} else {
		CLISimulation* sim = new CLISimulation(simvarsFile);       
		sim->runSim();
	}
}
