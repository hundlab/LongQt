#include "runwidget.h"
#include <logger.h>
#include <settingsIO.h>
#include <QFileDialog>
#include <QGridLayout>
#include <QLabel>
#include <QMessageBox>
#include <QTextStream>
#include "settingsIO.h"
#include "ui_runwidget.h"

using namespace std;
using namespace LongQt;

RunWidget::RunWidget(shared_ptr<Protocol> proto, QWidget* parent)
    : QWidget(parent), ui(new Ui::RunWidget), watcher(&runner, this) {
  ui->setupUi(this);
  this->proto = proto;

  connect(&watcher, SIGNAL(finished()), this, SLOT(finish()));
  connect(&watcher, SIGNAL(finished()), ui->progressBar, SLOT(reset()));
  connect(&watcher, SIGNAL(progressRangeChanged(int, int)), ui->progressBar,
          SLOT(setRange(int, int)));
  connect(&watcher, SIGNAL(progressValueChanged(int)), ui->progressBar,
          SLOT(setValue(int)));
  connect(ui->cancelButton, &QPushButton::clicked, this, &RunWidget::cancel);
}
void RunWidget::setProto(shared_ptr<Protocol> proto) { this->proto = proto; }
void RunWidget::write_note() {
  QScopedPointer<QFile> note_file(new QFile(
      QString(proto->getDataDir().c_str()) + "/" + ui->noteBoxName->text()));
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
  SettingsIO::getInstance()->writeSettings(
      (this->proto->getDataDir() + "/" + this->proto->simvarfile).c_str(),
      this->proto);
  emit running();
}

void RunWidget::on_saveButton_clicked() {
  QDir startDir = this->proto->getDataDir().c_str();
  startDir.cdUp();
  QString filename = QFileDialog::getSaveFileName(
      this, "Save Simulation Settings", startDir.absolutePath(),
      "XML files (*.xml)");
  if (filename != "") {
    SettingsIO::getInstance()->writeSettings(filename, this->proto);
  }
}

void RunWidget::cancel() {
  watcher.cancel();
  QMessageBox::critical(this, "Cancel",
                        "Simulation canceled!\n Wait for current "
                        "simulations to stop running");
  emit canceled();
}
void RunWidget::finish() {
  ui->runButton->setEnabled(true);
  runner.clear();
  emit finished();
}
