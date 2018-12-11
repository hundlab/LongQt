#include <QMessageBox>
#include <QStringList>

#include "ionchannelconfig.h"
#include "ui_pvarmenu.h"

#include "pvarsgrid.h"
using namespace std;
using namespace LongQt;

IonChannelConfig::IonChannelConfig(QTableView *view,
                                   shared_ptr<GridProtocol> proto,
                                   QWidget *parent)
    : QWidget(parent), ui(new Ui::PvarMenu) {
  ui->setupUi(this);
  this->view = view;
  this->model = (GridModel *)view->model();
  this->proto = proto;
  this->updateList();
  ui->treeWidget->addAction(ui->actionDelete);
  ui->treeWidget->addAction(ui->actionShow_Cells);
  connect(this->model, &GridModel::cellChanged, this,
          &IonChannelConfig::changeCell);
}

IonChannelConfig::~IonChannelConfig() { delete ui; }

void IonChannelConfig::updateList() {
  QStringList toAdd;
  this->ui->treeWidget->clear();
  for (auto &pvar : this->proto->pvars()) {
    new QTreeWidgetItem(
        ui->treeWidget,
        QString(pvar.second->IonChanParam::str(pvar.first).c_str())
            .split("\t"));
  }
}

void IonChannelConfig::changeCell(shared_ptr<Cell> cell) {
  if (this->addmenu) addmenu->changeCell(cell);
  this->updateList();
}
void IonChannelConfig::changeProto(shared_ptr<GridProtocol> proto) {
  this->proto = proto;
  if (this->addmenu) addmenu->changeProto(proto);
  this->updateList();
}

void IonChannelConfig::on_actionDelete_triggered() {
  QList<QTreeWidgetItem *> items = ui->treeWidget->selectedItems();
  for (auto item : items) {
    QString name = item->data(0, Qt::DisplayRole).toString();
    this->proto->pvars().erase(name.toStdString());
    int row = ui->treeWidget->indexOfTopLevelItem(item);
    ui->treeWidget->takeTopLevelItem(row);
    delete item;
  }
}

void IonChannelConfig::on_actionShow_Cells_triggered() {
  QList<QTreeWidgetItem *> items = ui->treeWidget->selectedItems();
  QString text;
  for (auto item : items) {
    QString name = item->data(0, Qt::DisplayRole).toString();
    text += this->proto->pvars()
                .at(name.toStdString())
                ->str(name.toStdString())
                .c_str();
  }
  QMessageBox msgBox;
  msgBox.setText(text);
  msgBox.exec();
}
void IonChannelConfig::on_addButton_triggered() {
  this->addmenu = new AddGridCellPvar(this->view, this->proto, this);
  connect(addmenu, &AddGridCellPvar::pvarsChanged,
          [this]() { this->updateList(); });
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
