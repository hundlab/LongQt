#include "varmenu.h"
#include "lqgrideditor.h"
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

    LQGridEditor window;
    window.show();
    return a.exec();
}
