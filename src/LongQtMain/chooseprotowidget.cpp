#include <QFileDialog>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QRadioButton>
#include <QSlider>
#include <QVBoxLayout>

#include "cellutils.h"
#include "chooseprotowidget.h"
#include "currentClampProtocol.h"
#include "settingsIO.h"
#include "ui_chooseprotowidget.h"
using namespace std;
using namespace LongQt;

ChooseProtoWidget::ChooseProtoWidget(QWidget* parent)
    : QWidget(parent), ui(new Ui::ChooseProtoWidget) {
  ui->setupUi(this);
  this->Initialize();
}

void ChooseProtoWidget::Initialize() {
  this->clampType = new ProtoChooser(ui->protocolType);
  this->cellOptions =
      new SimvCellOpts(clampType->proto, "cell_option", ui->cellOptions);
  this->cellType = new SimvCell(clampType->proto, "celltype", ui->cellType);

  connect(cellType, &SimvCell::cellChanged, this,
          &ChooseProtoWidget::cellChanged);
  connect(clampType, &ProtoChooser::cellChanged, this,
          &ChooseProtoWidget::cellChanged);
  connect(clampType, &ProtoChooser::protocolChanged, this,
          &ChooseProtoWidget::protocolChanged);
  connect(ui->citationButton, &QPushButton::clicked, [this]() {
      QMessageBox mb(QMessageBox::NoIcon,
                     this->getCurrentProto()->cell()->type(),
                     this->getCurrentProto()->cell()->citation()
                     , QMessageBox::Ok, this);
      mb.setStyleSheet(" QLabel {"
                       " min-width: 40em;"
                       "}");
      mb.setTextInteractionFlags(Qt::TextSelectableByMouse);
      mb.exec();
  });
}
shared_ptr<Protocol> ChooseProtoWidget::getCurrentProto() {
  return clampType->proto;
}

void ChooseProtoWidget::updateMenu() {
  clampType->update_ui();
  cellType->update_ui();
  cellOptions->update_ui();
}

void ChooseProtoWidget::changeCell(shared_ptr<Cell> cell) {
  this->cellOptions->changeCell(cell);
  this->updateMenu();
}

void ChooseProtoWidget::changeProto(std::shared_ptr<Protocol> proto) {
  clampType->changeProto(proto);
  cellType->changeProto(proto);
  cellOptions->changeProto(proto);
  this->updateMenu();
}

void ChooseProtoWidget::on_readSettings_clicked() {
  QString fileName = QFileDialog::getOpenFileName(
      this, "Choose Settings file", clampType->proto->datadir.absolutePath());
  if (!fileName.isEmpty()) {
    SettingsIO* settingsMgr = SettingsIO::getInstance();
    clampType->changeProto(
        settingsMgr->readSettings(fileName, clampType->proto));
    emit protocolChanged(clampType->proto);
    emit cellChanged(clampType->proto->cell());
    this->updateMenu();
  }
}
