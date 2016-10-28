#include "varmenu.h"
#include "gridSettup.h"
#include <QApplication>
#include <QStandardPaths> 
#include <QFileDialog>
#include <QDir>
#include <QMessageBox>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QCoreApplication::setOrganizationName("Hund lab BME OSU");
    QCoreApplication::setOrganizationDomain("http://hundlab.org");
    QCoreApplication::setApplicationName("LQGridEditor");

    QDir location = QStandardPaths::standardLocations(QStandardPaths::DocumentsLocation).first();
    location = QDir(QFileDialog::getExistingDirectory(Q_NULLPTR,"Choose Data Directory", location.absolutePath()));
	gridProtocol* proto = new gridProtocol();
 
    gridSetupWidget window(proto, location);
    window.show();
    return a.exec();
}
