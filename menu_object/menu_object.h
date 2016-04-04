#ifndef MENU_OBJECT_H
#define MENU_OBJECT_H

#include <QDir>
#include <QWidget>

#include "protocol.h"

class menu_object {
  public:
    string name;
    virtual QWidget* getWidget() = 0;
    virtual void reset() = 0;
    virtual void setWorkingDir(QDir dir) = 0;
    virtual void leave() = 0;
    virtual void changeProto(Protocol* proto) = 0;
};

#endif
