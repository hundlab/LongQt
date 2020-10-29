#include <iostream>
#include <QApplication>
#include <QDir>
#include <QFileDialog>
#include <QStandardPaths>
#include "graph.h"
#include "guiUtils.h"
#include "logger.h"

int main(int argc, char* argv[]) {
  QApplication a(argc, argv);
  QCoreApplication::setOrganizationName("Hund lab BME OSU");
  QCoreApplication::setOrganizationDomain("http://hundlab.org");
  QCoreApplication::setApplicationName("LongQtGrapher");
  QCoreApplication::setApplicationVersion(GuiUtils::version);

  QString location = QFileDialog::getExistingDirectory(
      Q_NULLPTR, "Choose Data Directory",
      QStandardPaths::standardLocations(QStandardPaths::DocumentsLocation)
          .first());
  if(location == "") {
      return 0;
  }

  LongQt::Logger::getInstance()->STDOut(&std::cout);

  Grapher* window;
  try {
    window = new Grapher(QDir(location));
    window->showMaximized();
  } catch (std::runtime_error&) {
    return 0;
  }

  return a.exec();
}
