#ifndef chooseProtoWidget_H
#define chooseProtoWidget_H

#include <QWidget>
#include <QComboBox>
#include <QSlider>
#include <QLabel>

#include "protocol.h"

using namespace std;

class chooseProtoWidget : public QWidget {
Q_OBJECT
  public:
    chooseProtoWidget(QWidget* parent = 0);
    Protocol* getCurrentProto();
    void Initialize(QWidget* parent = 0);
    void setCurrentProto(Protocol* proto);
  signals:
    void protocolChanged(Protocol*);
    void cell_type_changed();
  private:
    Protocol* proto;
    QWidget* parent;
    QSlider* clampType;
    QLabel* voltage;
    QLabel* current;
    QComboBox* cell_type;
  private slots:
    void changeProto(int value);
    void changeCell(QString name);
  public slots:
    void cellChangedSlot();
};

#endif
