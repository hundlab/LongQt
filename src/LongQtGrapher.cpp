#include "graph.h"
#include <QFileDialog>
#include <QApplication>
#include <QStandardPaths> 
#include <QDir>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QCoreApplication::setOrganizationName("Hund lab BME OSU");
    QCoreApplication::setOrganizationDomain("http://hundlab.org");
    QCoreApplication::setApplicationName("LongQtGrapher");

    QDir location = QStandardPaths::standardLocations(QStandardPaths::DocumentsLocation).first();
    location = QDir(QFileDialog::getExistingDirectory(Q_NULLPTR,"Choose Data Directory", location.absolutePath()));
    Grapher* window;
    try {
        window = new Grapher(location);
        window->showMaximized();
    } catch (std::runtime_error&) {
        return 0;
    }

    return a.exec();
}
