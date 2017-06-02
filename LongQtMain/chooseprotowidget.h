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

using namespace std;

namespace Ui {
class ChooseProtoWidget;
}

class ChooseProtoWidget : public QWidget {
Q_OBJECT
  public:
    ChooseProtoWidget(QWidget* parent = 0);
    Protocol* getCurrentProto();
    void Initialize();
  signals:
    void protocolChanged(Protocol*);
    void cell_type_changed();
  private:
	Ui::ChooseProtoWidget* ui;
    Protocol* proto;
    QWidget* parent;
    QButtonGroup* clampType;
//    QComboBox* ui::cellType;
    QString defaultCell;
  private slots:
    void on_cellType_currentIndexChanged(QString name);
  public slots:
    void changeProto(int value);
    void cellChangedSlot();
    void resetProto();
};

#endif
