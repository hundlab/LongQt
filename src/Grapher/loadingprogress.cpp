#include "loadingprogress.h"
#include <QCloseEvent>
#include <QMessageBox>
#include "ui_loadingprogress.h"

LoadingProgressDialog::LoadingProgressDialog(QSet<int> trials, QWidget* parent)
    : QDialog(parent), ui(new Ui::LoadingProgressDialog) {
  ui->setupUi(this);
  this->trials = trials;
  if (trials.size() >= 1000) {
    for (auto trial : this->trials) {
      if (trial > 1000) {
        this->excludedTrials.insert(trial);
      }
    }
  }
  this->createOptions();
}

void LoadingProgressDialog::createOptions() {
  int i = 0;
  int menuLength = 10;
  QList<int> orderedTrials = QList<int>::fromSet(this->trials);
  std::sort(orderedTrials.begin(), orderedTrials.end());
  for (auto& trial : orderedTrials) {
    QCheckBox* check;
    check = new QCheckBox("Trial " + QString::number(trial));
    this->trialCheckboxes.insert(trial, check);
    ui->checkBoxLayout->addWidget(check, 1 + i / menuLength, i % menuLength);
    check->setChecked(true);
    connect(check, &QCheckBox::stateChanged, [this, trial](int state) {
      switch (state) {
        case 0:
          this->excludedTrials.insert(trial);
          break;
        case 2:
          this->excludedTrials.remove(trial);
          break;
      }
    });
    i++;
  }
}

QSet<int> LoadingProgressDialog::getExcludes() const {
  return this->excludedTrials;
}

void LoadingProgressDialog::closeEvent(QCloseEvent* e) {
  e->accept();
  this->reject();
}
LoadingProgressDialog::~LoadingProgressDialog() { delete ui; }

void LoadingProgressDialog::on_skipButton_clicked() {
  for (auto trial = this->trialCheckboxes.begin();
       trial != this->trialCheckboxes.end(); trial++) {
    trial.value()->setChecked(false);
  }
}
void LoadingProgressDialog::on_loadAll_clicked() {
  for (auto trial = this->trialCheckboxes.begin();
       trial != this->trialCheckboxes.end(); trial++) {
    trial.value()->setChecked(true);
  }
}
void LoadingProgressDialog::on_loadFreqButton_clicked() {
  int trialFreq = ui->trialSpinBox->value();
  int position = 0;
  auto current = this->trialCheckboxes.find(position);
  while (current != this->trialCheckboxes.end()) {
    current.value()->setChecked(true);
    position += trialFreq;
    current = this->trialCheckboxes.find(position);
  }
}
