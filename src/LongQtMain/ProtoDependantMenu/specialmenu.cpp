#include "specialmenu.h"
#include "currentClampProtocol.h"
#include "gridProtocol.h"
#include "voltageClampProtocol.h"

#include "gridsetupwidget.h"
#include "oneitemlayout.h"
#include "stimulationsettings.h"
#include "voltageclampsetupwidget.h"

SpecialMenu::SpecialMenu(QWidget *parent) : QWidget(parent) {
  //{CurrentClamp::name, [this](auto proto) {}},
  specialMenus = {
      {LQ::CurrentClamp::name,
       [this](auto proto) {
         auto widg = new StimulationSettings(proto, this);
         connect(this, &SpecialMenu::cellChangedInternal, widg,
                 &StimulationSettings::changeCell);
         return widg;
       }},
      {LQ::VoltageClamp::name,
       [this](auto proto) {
         return new VoltageClampSetupWidget(
             std::static_pointer_cast<LQ::VoltageClamp>(proto), this);
       }},
      {LQ::GridProtocol::name, [this](std::shared_ptr<LQ::Protocol> proto) {
         auto grid = new GridSetupWidget(
             std::static_pointer_cast<LQ::GridProtocol>(proto));
         connect(grid, &GridSetupWidget::cellChanged, this,
                 &SpecialMenu::cellChanged);
         return grid;
       }}};
  names = {{LQ::CurrentClamp::name, "Pacing Setup"},
           {LQ::VoltageClamp::name, "Voltage Clamp Setup"},
           {LQ::GridProtocol::name, "Grid Setup"}};

  new OneItemLayout(this);
}

void SpecialMenu::changeProto(std::shared_ptr<LQ::Protocol> proto) {
  if (this->specialMenus.count(proto->type()) > 0) {
    this->show();
    this->layout()->addWidget(specialMenus[proto->type()](proto));
    emit nameChanged(this->names[proto->type()]);
  } else {
    emit noWidget();
    this->hide();
  }
}

void SpecialMenu::changeCell(std::shared_ptr<LQ::Cell> cell) {
  emit cellChangedInternal(cell);
}
