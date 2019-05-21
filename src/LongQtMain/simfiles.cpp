#include "simfiles.h"
#include <list>
#include <string>
#include "cellutils_core.h"

namespace LQ = LongQt;

SimFiles::SimFiles(std::shared_ptr<LongQt::Protocol> initial_proto,
                   QWidget* parent)
    : SimvarMenu(initial_proto, parent) {
  this->setNames({{"cellStateFile", "simvarfile"},
                  {"cellStateDir", "datadir"},
                  {"readCellState", "writeCellState"}});
  QCheckBox* read = this->findChild<QCheckBox*>("readCellState");
  QWidget* cellStateDir = this->findChild<QWidget*>("cellStateDir");
  connect(read, &QCheckBox::toggled, cellStateDir, &QWidget::setEnabled);
  cellStateDir->setEnabled(
      LQ::CellUtils::stob(initial_proto->parsStr("cellStateDir")));

  QCheckBox* write = this->findChild<QCheckBox*>("writeCellState");
  QWidget* cellStateFile = this->findChild<QWidget*>("cellStateFile");
  connect(write, &QCheckBox::toggled, cellStateFile, &QWidget::setEnabled);
  cellStateFile->setEnabled(
      LQ::CellUtils::stob(initial_proto->parsStr("writeCellState")));
  this->update_menu();
}
