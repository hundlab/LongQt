#include <QStringList>
#include <QMessageBox>

#include "ionchannelconfig.h"
#include "ui_pvarmenu.h"

#include "pvarsgrid.h"

IonChannelConfig::IonChannelConfig(QTableView* view, shared_ptr<GridProtocol> proto, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PvarMenu)
{
    ui->setupUi(this);
    this->view = view;
    this->model = (GridModel*)view->model();
    this->proto = proto;
    this->updateList();
    ui->listWidget->addAction(ui->actionDelete);
    ui->listWidget->addAction(ui->actionShow_Cells);
    connect(this->model, &GridModel::cellChanged, this, &IonChannelConfig::changeCell);
}

IonChannelConfig::~IonChannelConfig()
{
    delete ui;
}

void IonChannelConfig::updateList() {
    QStringList toAdd;
    this->ui->listWidget->clear();
    for(auto& pvar : this->proto->pvars()) {
        toAdd += pvar.second->IonChanParam::str(pvar.first).c_str();
    }
    this->ui->listWidget->addItems(toAdd);
}

void IonChannelConfig::changeCell(Cell* cell) {
    if(this->addmenu)
        addmenu->changeCell(cell);
    this->updateList();
}
void IonChannelConfig::changeProto(shared_ptr<GridProtocol> proto) {
    this->proto = proto;
    if(this->addmenu)
        addmenu->changeProto(proto);
    this->updateList();
}



void IonChannelConfig::on_actionDelete_triggered() {
    QList<QListWidgetItem *> items = ui->listWidget->selectedItems();
    for(auto item: items) {
        QString name = item->data(Qt::DisplayRole).toString().split("\t")[0];
        this->proto->pvars().erase(name.toStdString());
        int row = ui->listWidget->row(item);
        ui->listWidget->takeItem(row);
        delete item;
    }
}

void IonChannelConfig::on_actionShow_Cells_triggered() {
    QList<QListWidgetItem *> items = ui->listWidget->selectedItems();
    QString text;
    for(auto item: items) {
        QString name = item->data(Qt::DisplayRole).toString().split("\t")[0];
        text += this->proto->pvars().at(name.toStdString())->str(name.toStdString()).c_str();
    }
    QMessageBox msgBox;
    msgBox.setText(text);
    msgBox.exec();
}
void IonChannelConfig::on_addButton_triggered() {
    this->addmenu = new AddGridCellPvar(this->view,this->proto,this);
    connect(addmenu, &AddGridCellPvar::pvarsChanged, [this] () {
        this->updateList();
    });
    addmenu->show();
}

void IonChannelConfig::on_removeButton_triggered() {
    this->on_actionDelete_triggered();
}

void IonChannelConfig::on_infoButton_triggered() {
    this->on_actionShow_Cells_triggered();
}

void IonChannelConfig::on_refreshButton_triggered() {
    this->proto->pvars().calcIonChanParams();
}
