#include <QApplication>
#include <appparser.h>

int main(int argc, char *argv[])
{
	QCoreApplication* app;
	app = new QApplication(argc, argv);
	app->setOrganizationName("Hund lab BME OSU");
	app->setOrganizationDomain("http://hundlab.org");
	app->setApplicationName("LongQt");
	app->setApplicationVersion("v0.2");

	AppParser parser(app);
	parser.process();
	parser.start();

	return app->exec();
}

