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

using namespace std;

namespace Ui {
class ChooseProtoWidget;
}

class ChooseProtoWidget : public QWidget {
Q_OBJECT
  public:
    ChooseProtoWidget(QWidget* parent = 0);
    shared_ptr<Protocol> getCurrentProto();
    void Initialize();
  signals:
    void protocolChanged(shared_ptr<Protocol>);
    void cellChanged(shared_ptr<Cell>);
  private:
	Ui::ChooseProtoWidget* ui;
    shared_ptr<Protocol> proto;
    QWidget* parent;
    QButtonGroup* clampType;
//    QComboBox* ui::cellType;
    QString defaultCell;
	QMap<int,string> protoNumMap;
	void updateMenu();
  private slots:
    void on_cellType_currentIndexChanged(QString name);
  public slots:
    void changeProto(int value);
  	void changeProto(string name);
	void changeProto(shared_ptr<Protocol>, bool raise = false);
    void changeCell(shared_ptr<Cell>);
    void resetProto();
	void on_readSettings_clicked();
};

#endif
