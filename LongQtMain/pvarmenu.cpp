#include "guiUtils.h"
#include "pvarmenu.h"
#include "ui_pvarmenu.h"
#include "cellpvars.h"

#include <QMessageBox>

PvarMenu::PvarMenu(shared_ptr<Protocol> proto, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PvarMenu)
{
    ui->setupUi(this);
    this->proto = proto;
    ui->listWidget->addAction(ui->actionDelete);
    ui->listWidget->addAction(ui->actionShow_Cells);
    this->updateList();
}

PvarMenu::~PvarMenu()
{
    delete ui;
}

void PvarMenu::updateList() {
    QStringList toAdd;
    this->ui->listWidget->clear();
    for(auto& pvar : this->proto->pvars()) {
        toAdd += pvar.second->IonChanParam::str(pvar.first).c_str();
    }
    this->ui->listWidget->addItems(toAdd);
}

void PvarMenu::changeProto(shared_ptr<Protocol> proto) {
    this->proto = proto;
    if(this->addmenu)
        addmenu->changeProto(proto);
    this->updateList();
}

void PvarMenu::changeCell(Cell* cell) {
    if(this->addmenu)
        addmenu->changeCell(cell);
    this->updateList();
}

void PvarMenu::on_actionDelete_triggered() {
    QList<QListWidgetItem *> items = ui->listWidget->selectedItems();
    for(auto item: items) {
        QString name = item->data(Qt::DisplayRole).toString().split("\t")[0];
        this->proto->pvars().erase(name.toStdString());
        int row = ui->listWidget->row(item);
        ui->listWidget->takeItem(row);
        delete item;
    }
}

void PvarMenu::on_actionShow_Cells_triggered() {
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

void PvarMenu::on_addButton_triggered() {
    this->addmenu = new AddSingleCellPvar(this->proto,this);
    connect(addmenu, &AddSingleCellPvar::pvarsChanged, [this] () {
        this->updateList();
    });
    addmenu->show();
}

void PvarMenu::on_removeButton_triggered() {
    this->on_actionDelete_triggered();
}

void PvarMenu::on_infoButton_triggered() {
    this->on_actionShow_Cells_triggered();
}

void PvarMenu::on_refreshButton_triggered() {
    this->proto->pvars().calcIonChanParams();
}
