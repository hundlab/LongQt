#include "addsinglecellpvar.h"
#include "ui_addsinglecellpvar.h"
#include "cellpvars.h"
#include "guiUtils.h"

AddSingleCellPvar::AddSingleCellPvar(shared_ptr<Protocol> proto, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddSingleCellPvar)
{
    ui->setupUi(this);

    this->proto = proto;
    this->updateIonChannelType();
    this->pvarsDescriptions = GuiUtils::readMap(":/hoverText/pvarsDescriptions.txt");
}

AddSingleCellPvar::~AddSingleCellPvar()
{
    delete ui;
}

void AddSingleCellPvar::updateIonChannelType() {
    QRegExp allowed_vars = QRegExp("Factor");
    QStringList toAdd;
    ui->ionChannelType->clear();
    for(auto& pvarName : this->proto->cell()->getConstants()) {
        if(allowed_vars.indexIn(pvarName.c_str()) != -1) {
            toAdd += pvarName.c_str();
        }
    }
    toAdd.removeDuplicates();
    ui->ionChannelType->addItems(toAdd);
}

void AddSingleCellPvar::on_randomize_stateChanged(int state) {
    ui->normalDist->setVisible(state);
    ui->lognormalDist->setVisible(state);
    ui->mean->setVisible(state);
    ui->meanLabel->setVisible(state);
    ui->stdDev->setVisible(state);
    ui->stdDevLabel->setVisible(state);
    ui->startVal->setVisible(!state);
    ui->startValLabel->setVisible(!state);
    ui->incAmt->setVisible(!state);
    ui->incAmtLabel->setVisible(!state);
}

void AddSingleCellPvar::on_normalDist_toggled(bool checked) {
    if(checked) {
        if(ui->mean->value() == 0) {
            ui->mean->setValue(1);
        }
    } else {
        if(ui->mean->value() == 1) {
            ui->mean->setValue(0);
        }
    }
}
void AddSingleCellPvar::on_addButton_clicked()
{
    string type = ui->ionChannelType->currentText().toStdString();
    CellPvars::IonChanParam toAdd = CellPvars::IonChanParam();
    if(ui->randomize->checkState() == 0) {
        toAdd.dist = CellPvars::Distribution::none;
        toAdd.val[0] = ui->startVal->value();
        toAdd.val[1] = ui->incAmt->value();
    } else {
        toAdd.val[0] = ui->mean->value();
        toAdd.val[1] = ui->stdDev->value();
        if(ui->normalDist->isChecked()) {
            toAdd.dist = CellPvars::Distribution::normal;
        } else {
            toAdd.dist = CellPvars::Distribution::lognormal;
        }
    }
    this->proto->pvars().insert(type,toAdd);
    emit pvarsChanged();
}

void AddSingleCellPvar::changeProto(shared_ptr<Protocol> proto) {
    this->proto = proto;
}

void AddSingleCellPvar::changeCell(shared_ptr<Cell>) {
    this->updateIonChannelType();
}
