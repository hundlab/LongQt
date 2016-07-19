#include "dialog.h"
#include <QApplication>
#include <QStandardPaths> 
#include <QDir>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QCoreApplication::setOrganizationName("Hund lab BME OSU");
    QCoreApplication::setOrganizationDomain("http://hundlab.org");
    QCoreApplication::setApplicationName("My Concurrent Model");

    QDir location = QStandardPaths::standardLocations(QStandardPaths::DocumentsLocation).first();
    location = QDir(QFileDialog::getExistingDirectory(Q_NULLPTR,"Choose Data Directory", location.absolutePath()));
    Dialog window(location);
    window.show();
    return a.exec();
}
