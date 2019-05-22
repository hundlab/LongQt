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
  auto createStimSet = [this](auto proto) {
    auto widg = new StimulationSettings(proto, this);
    connect(this, &SpecialMenu::cellChangedInternal, widg,
            &StimulationSettings::changeCell);
    return widg;
  };
  specialMenus = {{LQ::CurrentClamp::name, createStimSet},
                  {LQ::GridProtocol::name, createStimSet},
                  {LQ::VoltageClamp::name, [this](auto proto) {
                     return new VoltageClampSetupWidget(
                         std::static_pointer_cast<LQ::VoltageClamp>(proto),
                         this);
                   }}};
  names = {{LQ::CurrentClamp::name, "Pacing Setup"},
           {LQ::VoltageClamp::name, "Voltage Clamp Setup"},
           {LQ::GridProtocol::name, "Pacing Setup"}};

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
