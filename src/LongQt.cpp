#include <appparser.h>
#include <QApplication>
#include "guiUtils.h"

int main(int argc, char* argv[]) {
  QCoreApplication* app;
  app = new QApplication(argc, argv);
  app->setOrganizationName("Hund lab BME OSU");
  app->setOrganizationDomain("http://hundlab.org");
  app->setApplicationName("LongQt");
  app->setApplicationVersion(GuiUtils::version);

  AppParser parser(app);
  parser.process();
  parser.start();

  return app->exec();
}
