#include "runwidget.h"
#include "ui_runwidget.h"
#include <QGridLayout>
#include <QLabel>
#include <QtConcurrent>

RunWidget::RunWidget(Protocol* proto, QDir working_dir, QWidget* parent) :
	QWidget(parent),
	ui(new Ui::RunWidget)
{
	ui->setupUi(this);
	this->parent = parent;
	this->proto = proto;
	this->working_dir = working_dir;

	connect(&watcher,SIGNAL(finished()),this,SLOT(finish()));
	connect(&watcher,SIGNAL(finished()),ui->progressBar,SLOT(reset()));
	connect(&watcher,SIGNAL(progressRangeChanged(int,int)),ui->progressBar,SLOT(setRange(int,int)));
	connect(&watcher,SIGNAL(progressValueChanged(int)),ui->progressBar,SLOT(setValue(int)));
}
void RunWidget::setProto(Protocol* proto) {
	this->proto = proto;
}
void RunWidget::on_runButton_clicked() {
	int i = 0;
	ui->runButton->setEnabled(false);
	working_dir.mkpath(working_dir.absolutePath());
	QFile* note_file = new QFile(working_dir.absolutePath()+ "/" + ui->noteBoxName->text() + ".txt");
	note_file->open(QIODevice::WriteOnly|QIODevice::Text);
	if(note_file->isOpen()) {
		QTextStream out(note_file);
		out << ui->noteBox->toPlainText();
	} else {
		cerr << "note file could not be opened";
	}
	vector.clear();

	for( i = 0; i < proto->numtrials; i++) {
		proto->setTrial(i);
		proto->readfile = "r"+ to_string(i) + ".dat"; // File to read SV ICs
		proto->savefile = "s"+ to_string(i) + ".dat"; // File to save final SV
		proto->propertyoutfile = "dt%d_%s" + string(".dat");
		proto->dvarsoutfile = "dt%d_dvars" + string(".dat");
		proto->finalpropertyoutfile = "dss%d_%s" + string(".dat");
		proto->finaldvarsoutfile = "dss%d_pvars" + string(".dat");
		vector.append(proto->clone());
	}

	QFuture<void> next = QtConcurrent::map(vector,[] (Protocol* p) {
			if(p != NULL) {
			p->runTrial();
			}
			});  // pass vector of protocols to QtConcurrent

	watcher.setFuture(next);
	emit running();
}
void RunWidget::setWorkingDir(QDir& dir) {
	working_dir = dir;
}
void RunWidget::cancel() {
	watcher.cancel();
	ui->runButton->setEnabled(true);
	emit canceled();
}
void RunWidget::finish() {
	ui->runButton->setEnabled(true);
	emit finished();
}
