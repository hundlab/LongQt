#include "heartcellsim.h"
#include "CLISimulation.h"
#include "appparser.h"
#include "logger.h"

#include <QApplication>
#include <QMessageBox>
#include <QSettings>

void noMessageOutput(QtMsgType type, const char *msg)
{
     Q_UNUSED(type);
     Q_UNUSED(msg);
}

AppParser::AppParser(QCoreApplication* app):
    GUIOption("no-gui",
            QCoreApplication::translate("main", "Start LongQt in CLI mode")),
    licenseOption(QStringList() << "l" << "license",
            QCoreApplication::translate("main", "Print the license and exit")),
    verboseOption("verbose", "Print all info and error messages")
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
    parser.addOption(verboseOption);
    parser.addPositionalArgument("simvars_files", QCoreApplication::translate("main", "Specify files to read settings from. They will be processes serially in the order listed. If --no-gui is not set only the first file will be read."), "[simvars.xml...]");
}

void AppParser::process() {
    parser.process(*app);
    bool license = parser.isSet(licenseOption);
    bool verbose = parser.isSet(verboseOption);

    this->simvarsFiles = parser.positionalArguments();

    if(license) {
        this->showLicense();
        exit(0);
    }
    if(verbose) {
        auto logger = LongQt::Logger::getInstance();
        logger->STDOut(&std::cout);
    }
}

void AppParser::showLicense() {
    QFile* licenseFile = new QFile(":/LICENSE");
    licenseFile->open(QIODevice::ReadOnly|QIODevice::Text);
    if(licenseFile->isOpen()) {
        QTextStream licenseStream(licenseFile);
        qInfo(licenseStream.readAll().toStdString().c_str());
    }
}

void AppParser::start() {
    bool startCLI = parser.isSet(GUIOption);

    if(!startCLI) {
        QString simvarsFile = "";
        if(!simvarsFiles.isEmpty()) {
            simvarsFile = simvarsFiles.first();
        }
        Simulation* window = new Simulation(simvarsFile);
        window->show();
        QSettings settings;
        if(settings.value("showHelp",true).toBool()) {
            QFile* helpFile = new QFile(":/startMessage.txt");
            helpFile->open(QIODevice::ReadOnly|QIODevice::Text);
            QString showHelpText;
            if(helpFile->isOpen()) {
                QTextStream helpStream(helpFile);
                showHelpText = helpStream.readAll();
            }
            if(QMessageBox::Discard == QMessageBox::information(0,"Welcome!", showHelpText, QMessageBox::Ok|QMessageBox::Discard,QMessageBox::Ok)) {
                settings.setValue("showHelp",false);
            }
        }
    } else {
        CLISimulation* sim = new CLISimulation();
        sim->runSims(simvarsFiles);
    }
}
