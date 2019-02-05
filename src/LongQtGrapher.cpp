#include <QApplication>
#include <QDir>
#include <QFileDialog>
#include <QStandardPaths>
#include "graph.h"

int main(int argc, char* argv[]) {
  QApplication a(argc, argv);
  QCoreApplication::setOrganizationName("Hund lab BME OSU");
  QCoreApplication::setOrganizationDomain("http://hundlab.org");
  QCoreApplication::setApplicationName("LongQtGrapher");

  QString location = QFileDialog::getExistingDirectory(
      Q_NULLPTR, "Choose Data Directory",
      QStandardPaths::standardLocations(QStandardPaths::DocumentsLocation)
          .first());
  if(location == "") {
      return 0;
  }

  Grapher* window;
  try {
    window = new Grapher(QDir(location));
    window->showMaximized();
  } catch (std::runtime_error&) {
    return 0;
  }

  return a.exec();
}
