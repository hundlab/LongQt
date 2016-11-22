#include "loadingprogress.h"
#include "ui_loadingprogress.h"


LoadingProgressDialog::LoadingProgressDialog(QFileInfoList files, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LoadingProgressDialog)
{
    ui->setupUi(this);
    this->files = files;
    this->filesToLoad = this->files;
    this->createOptions();
}

void LoadingProgressDialog::createOptions() {
    int i = 0;
    int menuLength = 10;
    bool isMultiCell = false;
    for(auto& file : this->files) {
        QStringList nums;
        nums = file.baseName().split(QRegExp("\\D+"), QString::SkipEmptyParts);
        QCheckBox* check;
        if(nums.length() > 1) {
            isMultiCell = true;
            check = new QCheckBox("Cell " + nums[0] + " " + nums[1]);
            this->trials.insert(QPair<int,int>(nums[0].toInt(),nums[1].toInt()),check);
            ((QGridLayout*)ui->advanced->layout())->addWidget(check,1+i/menuLength,i%menuLength);
        } else if( nums.length() > 0) {
            check = new QCheckBox("Trial " + nums[0]);
            this->trials.insert(QPair<int,int>(nums[0].toInt(),0),check);
            ((QGridLayout*)ui->advanced->layout())->addWidget(check,1+i/menuLength,i%menuLength);
        } else {
            check = new QCheckBox("Cell");
            this->trials.insert(QPair<int,int>(0,0),check);
            ((QGridLayout*)ui->advanced->layout())->addWidget(check,1+i/menuLength,i%menuLength);
        }
        check->setChecked(true);
        connect(check,&QCheckBox::stateChanged, [this,file] (int state) {
            switch(state) {
            case 0:
                this->filesToLoad.removeOne(file);
            break;
            case 2:
                this->filesToLoad.append(file);
            break;
            }
        });
        i++;
    }
    this->setMultiCell(isMultiCell);
}

QFileInfoList LoadingProgressDialog::getFilesToLoad() {
    return this->filesToLoad;
}
bool LoadingProgressDialog::multiCell() {
    return this->m_multiCell;
}
void LoadingProgressDialog::setMultiCell(bool value) {
    this->m_multiCell = value;
    if(value) {
        ui->thLabel->hide();
        ui->columnLabel->show();
        ui->rowLabel->show();
        ui->columnSpinBox->show();
    } else {
        ui->thLabel->show();
        ui->columnLabel->hide();
        ui->rowLabel->hide();
        ui->columnSpinBox->hide();
    }
}
LoadingProgressDialog::~LoadingProgressDialog()
{
    delete ui;
}

void LoadingProgressDialog::on_continueButton_clicked() {
    this->accept();
}

void LoadingProgressDialog::on_skipButton_clicked() {
    for(auto trial = this->trials.begin(); trial != this->trials.end(); trial++) {
        trial.value()->setChecked(false);
    }
}
void LoadingProgressDialog::on_loadAll_clicked() {
    for(auto trial = this->trials.begin(); trial != this->trials.end(); trial++) {
        trial.value()->setChecked(true);
    }
}
void LoadingProgressDialog::on_loadFreqButton_clicked() {
    int columnFreq = ui->columnSpinBox->value();
    int rowFreq = ui->spinBox->value();
    if((columnFreq+rowFreq)==0) {
        return;
    }
    QPair<int,int> position(0,0);
    auto current = this->trials.find(position);
    while(current != this->trials.end()) {
        while(current != this->trials.end()) {
            current.value()->setChecked(true);
            position.first+=rowFreq;
            current = this->trials.find(position);
        }
        position.first=0;
        if(columnFreq == 0) {
            break;
        }
        position.second+=columnFreq;
        current = this->trials.find(position);
    }
}
