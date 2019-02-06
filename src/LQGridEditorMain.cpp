#include <QApplication>
#include "lqgrideditor.h"
#include "guiUtils.h"

int main(int argc, char *argv[]) {
  QApplication a(argc, argv);
  QCoreApplication::setOrganizationName("Hund lab BME OSU");
  QCoreApplication::setOrganizationDomain("http://hundlab.org");
  QCoreApplication::setApplicationName("LQGridEditor");
  QCoreApplication::setApplicationVersion(GuiUtils::version);

  LQGridEditor window;
  window.show();
  return a.exec();
}
