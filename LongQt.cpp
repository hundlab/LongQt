#include "heart_cell_sim.h"
#include <QApplication>
#include <QSettings>
#include <QMessageBox>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QCoreApplication::setOrganizationName("Hund lab BME OSU");
    QCoreApplication::setOrganizationDomain("http://hundlab.org");
    QCoreApplication::setApplicationName("LongQt");

    Simulation window;
    window.show();
    QSettings settings;
    if(settings.value("showHelp",true).toBool()
        &&QMessageBox::Ignore == 
        QMessageBox::information(0,"Welcome!", "LongQt is a program for modeling cardiac potentials. As you go through this program keep a few things in mind. First, if you would like more information about something hover your mouse above its name and information will pop up. Second, default values have been provided for all options so if you don't know what an option does it's ok to simply skip it. And finally have fun!\n If you would like to re-enable this text after discarding it use the Restore Defaults button in the About dialog."
        ,QMessageBox::Ok|QMessageBox::Ignore,QMessageBox::Ok)) {
        settings.setValue("showHelp",false);
    }

    return a.exec();
}
