/*
 * This class provides a way to switch between different protocols
 * it creates and returns the new protocol which the user selects
 */
#ifndef chooseProtoWidget_H
#define chooseProtoWidget_H

#include <QButtonGroup>
#include <QComboBox>
#include <QLabel>
#include <QPointer>
#include <QWidget>

#include "cellutils.h"
#include "protochooser.h"
#include "protocol.h"
#include "simvcell.h"
#include "simvcellopts.h"
namespace LQ = LongQt;

namespace Ui {
class ChooseProtoWidget;
}

class ChooseProtoWidget : public QWidget {
  Q_OBJECT
 public:
  ChooseProtoWidget(QWidget* parent = 0);
  std::shared_ptr<LQ::Protocol> getCurrentProto();
  void Initialize();
 signals:
  void protocolChanged(std::shared_ptr<LQ::Protocol>);
  void cellChanged(std::shared_ptr<LQ::Cell>);

 private:
  Ui::ChooseProtoWidget* ui;
  QPointer<ProtoChooser> clampType;
  QPointer<SimvCellOpts> cellOptions;
  QPointer<SimvCell> cellType;

  void updateMenu();
 public slots:
  void changeCell(std::shared_ptr<LQ::Cell>);
  void changeProto(std::shared_ptr<LQ::Protocol>);
  void on_readSettings_clicked();
};

#endif
