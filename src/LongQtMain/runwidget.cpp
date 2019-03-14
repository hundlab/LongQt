#include "runwidget.h"
#include <logger.h>
#include <settingsIO.h>
#include <QFileDialog>
#include <QGridLayout>
#include <QLabel>
#include <QTextStream>
#include "ui_runwidget.h"

using namespace std;
using namespace LongQt;

RunWidget::RunWidget(shared_ptr<Protocol> proto, QDir working_dir,
                     QWidget* parent)
    : QWidget(parent), ui(new Ui::RunWidget), watcher(&runner, this) {
  ui->setupUi(this);
  this->parent = parent;
  this->proto = proto;
  this->working_dir = working_dir;

  connect(&watcher, SIGNAL(finished()), this, SLOT(finish()));
  connect(&watcher, SIGNAL(finished()), ui->progressBar, SLOT(reset()));
  connect(&watcher, SIGNAL(progressRangeChanged(int, int)), ui->progressBar,
          SLOT(setRange(int, int)));
  connect(&watcher, SIGNAL(progressValueChanged(int)), ui->progressBar,
          SLOT(setValue(int)));
}
void RunWidget::setProto(shared_ptr<Protocol> proto) { this->proto = proto; }
void RunWidget::write_note() {
  QScopedPointer<QFile> note_file(
      new QFile(working_dir.absolutePath() + "/" + ui->noteBoxName->text()));
  note_file->open(QIODevice::WriteOnly | QIODevice::Text);
  if (note_file->isOpen()) {
    QTextStream out(note_file.data());
    out << ui->noteBox->toPlainText();
  } else {
    Logger::getInstance()->write("Note file could not be opened");
  }
  note_file->close();
}
void RunWidget::on_runButton_clicked() {
  ui->runButton->setEnabled(false);
  runner.setSims(this->proto);
  this->write_note();
  runner.run();
  emit running();
}

void RunWidget::on_saveButton_clicked() {
  QDir startDir = this->working_dir;
  startDir.cdUp();
  QString filename = QFileDialog::getSaveFileName(
      this, "Save Simulation Settings", startDir.absolutePath(), "XML files (*.xml)");
  if (filename != "") {
    SettingsIO::getInstance()->writeSettings(filename, this->proto);
  }
}

void RunWidget::setWorkingDir(QDir& dir) { working_dir = dir; }

void RunWidget::cancel() {
  watcher.cancel();
  //  ui->runButton->setEnabled(true);
  emit canceled();
}
void RunWidget::finish() {
  ui->runButton->setEnabled(true);
  runner.clear();
  emit finished();
}
