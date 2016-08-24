#include "loadingprogress.h"
#include "ui_loadingprogress.h"


LoadingProgressDialog::LoadingProgressDialog(QFileInfoList files, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LoadingProgressDialog)
{
    ui->setupUi(this);
    this->files = files;
    this->filesToLoad = this->files;
//    if(files.length() > 10) {
        this->createOptions();
//    }
    connect(ui->continueButton, &QPushButton::clicked, this, &LoadingProgressDialog::on_continueButton_clicked);
}

void LoadingProgressDialog::createOptions() {
    int i = 0;
    int menuLength = 10;
    for(auto& file : this->files) {
        QStringList nums;
        nums = file.baseName().split(QRegExp("\\D+"), QString::SkipEmptyParts);
        QCheckBox* check;
        if(nums.length() > 1) {
            check = new QCheckBox("Cell " + nums[0] + " " + nums[1]);
            ((QGridLayout*)ui->advanced->layout())->addWidget(check,1+i/menuLength,i%menuLength);
        } else if( nums.length() > 0) {
            check = new QCheckBox("Trial " + nums[0]);
            ((QGridLayout*)ui->advanced->layout())->addWidget(check,1+i/menuLength,i%menuLength);
        } else {
            check = new QCheckBox("Cell");
            ((QGridLayout*)ui->advanced->layout())->addWidget(check,1+i/menuLength,i%menuLength);
        }
        check->setChecked(true);
        this->trials.append(check);
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
}

QFileInfoList LoadingProgressDialog::getFilesToLoad() {
    return this->filesToLoad;
}

LoadingProgressDialog::~LoadingProgressDialog()
{
    delete ui;
}

void LoadingProgressDialog::on_continueButton_clicked() {
    this->accept();
}

void LoadingProgressDialog::on_skipButton_clicked() {
    this->reject();
}
