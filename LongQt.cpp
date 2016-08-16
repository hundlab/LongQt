#include "heart_cell_sim.h"
#include "argHandler.h"
#include "CLISimulation.h"
#include <QApplication>
#include <QSettings>
#include <QMessageBox>
#include <QCommandLineParser>

int main(int argc, char *argv[])
{
    QCoreApplication* app;
    app = new QApplication(argc, argv);
    app->setOrganizationName("Hund lab BME OSU");
    app->setOrganizationDomain("http://hundlab.org");
    app->setApplicationName("LongQt");
    app->setApplicationVersion("v0.1");

    QCommandLineParser parser;
    parser.setApplicationDescription("Copyright© 2016 Thomas J. Hund\nLicense: Revised BSD License\nEmail: thomas.hund@osumc.edu\nVersion 0.1\nContributers:\n\tThomas Hund\n\tBirce Ela Onal\n\tDanielle Beckley\n\tDaniel Gratz\n");
    parser.addHelpOption();
    parser.addVersionOption();
    QCommandLineOption GUIOption("no-gui", QCoreApplication::translate("main", "Start LongQt in CLI mode"));
    parser.addOption(GUIOption);
    QCommandLineOption licenseOption(QStringList() << "l" << "license", QCoreApplication::translate("main", "Print the license and exit"));
    parser.addOption(licenseOption);
    QCommandLineOption protoOption("proto",QCoreApplication::translate("main","Specify the protocol to use 0=CurrentClamp, 1=VoltageClamp, 2=Grid"),QCoreApplication::translate("main","num[0-2]"));
    parser.addOption(protoOption);
    QCommandLineOption pvarsFileOption("pvars",QCoreApplication::translate("main","Specify the file for setting model parameters or their random generation"),QCoreApplication::translate("main","pvars.txt"));
    parser.addOption(pvarsFileOption);
    QCommandLineOption dvarsFileOption("dvars",QCoreApplication::translate("main","Specify the file for choosing which variables will be writen out durring the simulation"),QCoreApplication::translate("main","dvars.txt"));
    parser.addOption(dvarsFileOption);
    QCommandLineOption mvarsFileOption("mvars",QCoreApplication::translate("main","Specify the file for choosing which variables will have properties measured about them e.g. Ampiltude on the Ca channel"),QCoreApplication::translate("main","mvars.txt"));
    parser.addOption(mvarsFileOption);
    QCommandLineOption simvarsFileOption("simvars",QCoreApplication::translate("main","Specify the file for setting simulation parameters e.g. number of trials"),QCoreApplication::translate("main","simvars.txt"));
    parser.addOption(simvarsFileOption);


    parser.process(*app);     
    bool startCLI = parser.isSet(GUIOption);
    bool license = parser.isSet(licenseOption);
    bool valid;
    int protoNum = parser.value(protoOption).toInt(&valid);
    if(!valid) {
        protoNum = -1;
    }
    QString pvarsFile = parser.value(pvarsFileOption);
    QString dvarsFile = parser.value(dvarsFileOption);
    QString mvarsFile = parser.value(mvarsFileOption);
    QString simvarsFile = parser.value(simvarsFileOption);

    if(license) {
        QFile* licenseFile = new QFile(":/LICENSE");
        licenseFile->open(QIODevice::ReadOnly|QIODevice::Text);
        if(licenseFile->isOpen()) {
            QTextStream licenseStream(licenseFile);
            cout << licenseStream.readAll().toStdString();
        }
        exit(0);
    }
    
    if(!startCLI) {
        Simulation* window = new Simulation(protoNum, simvarsFile, dvarsFile, mvarsFile, pvarsFile);
        window->show();
        QSettings settings;
        if(settings.value("showHelp",true).toBool() &&QMessageBox::Discard == QMessageBox::information(0,"Welcome!", "LongQt is a program for modeling cardiac potentials. As you go through this program keep a few things in mind. First, if you would like more information about something hover your mouse above its name and information will pop up. Second, default values have been provided for all options so if you don't know what an option does it's ok to simply skip it. And finally have fun!\n If you would like to re-enable this text after discarding it use the Restore Defaults button in the About dialog.", QMessageBox::Ok|QMessageBox::Discard,QMessageBox::Ok)) {
            settings.setValue("showHelp",false);
        }
    } else {
        CLISimulation* sim = new CLISimulation(protoNum, simvarsFile, dvarsFile, mvarsFile, pvarsFile);       
        sim->runSim();
    }
    return app->exec();
}

