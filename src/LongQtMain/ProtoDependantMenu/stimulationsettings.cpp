#include "stimulationsettings.h"

StimulationSettings::StimulationSettings(
    std::shared_ptr<LQ::Protocol> initial_proto, QWidget* parent)
    : SimvarMenu(initial_proto, parent) {
  this->setNames({{"stimt", "stimdur", "bcl", "tMax"},
                  {"stimval", "numstims", "paceflag"}});
  std::list<QString> toggleNames = {"stimdur", "stimt", "stimval", "bcl",
                                    "numstims"};
  QCheckBox* pace = this->findChild<QCheckBox*>("paceflag");
  for (auto name : toggleNames) {
    QWidget* widg = this->findChild<QWidget*>(name);
    connect(pace, &QCheckBox::toggled, widg, &QWidget::setEnabled);
  }
  this->update_menu();
}
