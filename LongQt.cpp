#include "heart_cell_sim.h"
#include "argHandler.h"
#include <QApplication>
#include <QSettings>
#include <QMessageBox>

int main(int argc, char *argv[])
{
    QCoreApplication* app;
    function<void(QCoreApplication*)> settupQApp = [] (QCoreApplication* app) {
        app->setOrganizationName("Hund lab BME OSU");
        app->setOrganizationDomain("http://hundlab.org");
        app->setApplicationName("LongQt");
        app->setApplicationVersion("v0.1");
    };
    argHandler handler;
    handler.setArgs(argc, argv);
    map<string,function<void(vector<string>)>> handlerMap;
    handlerMap["-no-gui"] = [&app, &argc, argv, settupQApp] (vector<string>) {
        app = new QCoreApplication(argc, argv);
        settupQApp(app);
    };
    handler.setHandlers(handlerMap);
    map<string,function<void(void)>> handleNotFoundMap;
    handleNotFoundMap["-no-gui"] = [&app, &argc, argv, settupQApp] () {
        app = new QApplication(argc, argv);
        settupQApp(app);
        Simulation* window = new Simulation();
        window->show();
        QSettings settings;
        if(settings.value("showHelp",true).toBool() &&QMessageBox::Discard == QMessageBox::information(0,"Welcome!", "LongQt is a program for modeling cardiac potentials. As you go through this program keep a few things in mind. First, if you would like more information about something hover your mouse above its name and information will pop up. Second, default values have been provided for all options so if you don't know what an option does it's ok to simply skip it. And finally have fun!\n If you would like to re-enable this text after discarding it use the Restore Defaults button in the About dialog.", QMessageBox::Ok|QMessageBox::Discard,QMessageBox::Ok)) {
            settings.setValue("showHelp",false);
        }
    };
    handler.setNotFoundHandlers(handleNotFoundMap);
    handler.parse();
    return app->exec();
}


