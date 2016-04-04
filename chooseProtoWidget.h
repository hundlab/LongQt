#ifndef chooseProtoWidget_H
#define chooseProtoWidget_H

#include <QWidget>
#include <QSlider>
#include <QLabel>

#include "protocol.h"

using namespace std;

class chooseProtoWidget : public QWidget {
Q_OBJECT
  public:
    chooseProtoWidget(QWidget* parent = 0);
    Protocol* getCurrentProto();
    void setCurrentProto(Protocol* proto);
  signals:
    void protocolChanged();
  private:
    Protocol* proto;
    QWidget* parent;
    QSlider* clampType;
    QLabel* voltage;
    QLabel* current;
  private slots:
    void changeProto(int value);
};

#endif
