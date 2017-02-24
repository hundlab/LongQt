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

class chooseProtoWidget : public QWidget {
Q_OBJECT
  public:
    chooseProtoWidget(QWidget* parent = 0);
    Protocol* getCurrentProto();
    void Initialize();
  signals:
    void protocolChanged(Protocol*);
    void cell_type_changed();
  private:
    Protocol* proto;
    QWidget* parent;
    QButtonGroup* clampType;
    QComboBox* cell_type;
    QString defaultCell;
  private slots:
    void changeProto(int value);
    void changeCell(QString name);
  public slots:
    void cellChangedSlot();
    void resetProto();
};

#endif
