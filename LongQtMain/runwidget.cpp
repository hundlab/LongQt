#include "runwidget.h"
#include "ui_runwidget.h"
#include <QGridLayout>
#include <QLabel>

RunWidget::RunWidget(shared_ptr<Protocol> proto, QDir working_dir, QWidget* parent) :
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
void RunWidget::setProto(shared_ptr<Protocol> proto) {
    this->proto = proto;
}
void RunWidget::write_note() {
    QScopedPointer<QFile> note_file(new QFile(working_dir.absolutePath()+ "/" + ui->noteBoxName->text()));
    note_file->open(QIODevice::WriteOnly|QIODevice::Text);
    if(note_file->isOpen()) {
        QTextStream out(note_file.data());
        out << ui->noteBox->toPlainText();
    } else {
        qWarning() << "note file could not be opened";
    }
    note_file->close();
}
void RunWidget::on_runButton_clicked() {
    ui->runButton->setEnabled(false);
    runner.setSims(this->proto);
    this->write_note();
    runner.run();
    watcher.setFuture(runner.getFuture());
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
    runner.clear();
    emit finished();
}
