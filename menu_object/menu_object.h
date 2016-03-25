#ifndef MENU_OBJECT_H
#define MENU_OBJECT_H

#include <QDir>
#include <QWidget>

class menu_object {
  public:
    string name;
    virtual QWidget* getWidget() = 0;
    virtual void reset() = 0;
    virtual void setWorkingDir(QDir dir) = 0;
    virtual void leave() = 0;
};

#endif
