/*
 * This class provides a way to switch between different protocols
 * it creates and returns the new protocol which the user selects
 */
#ifndef chooseProtoWidget_H
#define chooseProtoWidget_H

#include <QWidget>
#include <QComboBox>
#include <QButtonGroup>
#include <QLabel>

#include "protocol.h"
#include "cellutils.h"
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
    std::shared_ptr<LQ::Protocol> proto;
    QWidget* parent;
    QButtonGroup* clampType;
//    QComboBox* ui::cellType;
    QString defaultCell;
    QMap<int,std::string> protoNumMap;
	void updateMenu();
  private slots:
    void on_cellType_currentIndexChanged(QString name);
  public slots:
    void changeProto(int value);
    void changeProto(std::string name);
    void changeProto(std::shared_ptr<LQ::Protocol>, bool raise = false);
    void changeCell(std::shared_ptr<LQ::Cell>);
    void resetProto();
	void on_readSettings_clicked();
};

#endif
