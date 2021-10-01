#include "protochooser.h"
#include <QApplication>
#include <QLayout>
#include <QRadioButton>
#include "cellutils.h"
#include "currentClampProtocol.h"
#include "protocol.h"

using namespace LongQt;

ProtoChooser::ProtoChooser(QGroupBox* parent) : QObject(parent) {
  this->proto.reset(new CurrentClamp());
  this->group = new QButtonGroup(this);
  group->setExclusive(true);

  auto layout = parent->layout();
  for (int i = 0; i < layout->count(); ++i) {
    auto layoutItem = layout->itemAt(i);
    group->addButton(static_cast<QAbstractButton*>(layoutItem->widget()), i);
  }

  connect(group, QOverload<int>::of(&QButtonGroup::buttonPressed), this,
          &ProtoChooser::update_model);
}

ProtoChooser::~ProtoChooser() {}

void ProtoChooser::update_ui() {
  int typeNum = this->protoNums.indexOf(this->proto->type());
  auto button = group->button(typeNum);
  if (button) button->setChecked(true);
}

void ProtoChooser::changeProto(std::shared_ptr<Protocol> proto) {
  this->proto = proto;
  this->update_ui();
}

void ProtoChooser::update_model(int index) {
  auto newProto = CellUtils::protoMap.at(this->protoNums[index])();
  newProto->setCellStateDir(this->proto->getCellStateDir());
  newProto->setDataDirDirect(this->proto->getDataDir());
  auto old_cell = this->proto->cell();
  auto cellTypesList = newProto->cellOptions();
  if (std::any_of(cellTypesList.begin(), cellTypesList.end(),
                  [old_cell](auto& val) { return val == old_cell->type(); })) {
    newProto->cell(old_cell);
  }
  this->proto = newProto;

  emit protocolChanged(this->proto);
  if (proto->cell() != old_cell) {
    emit cellChanged(proto->cell());
  }
}
