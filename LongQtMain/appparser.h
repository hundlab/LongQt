/*
 * Uses QCommandLineParser to parse command line arguments and handles the results
 */
#ifndef APP_PARSER_H
#define APP_PARSER_H
#include <QCommandLineParser>
#include <QString>


class AppParser {
	QCommandLineParser parser;
	QCoreApplication* app;

	QCommandLineOption GUIOption;
	QCommandLineOption licenseOption;
	QCommandLineOption protoOption;
	QCommandLineOption pvarsFileOption;
	QCommandLineOption dvarsFileOption;
	QCommandLineOption mvarsFileOption;
	QCommandLineOption simvarsFileOption;

	public:
	AppParser(QCoreApplication* app);
	~AppParser() {};
	void process();
	void showLicense();
	void start();

	QString simvarsFile;
	QString mvarsFile;
	QString dvarsFile;
	QString pvarsFile;
};

#endif
