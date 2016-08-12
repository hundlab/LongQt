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
    Dialog* window;
    try {
        window = new Dialog(location);
        window->showMaximized();
    } catch (badFile) {
        return 0;
    }

    return a.exec();
}
