#include "menu_object.h"
#include "chooseProtoWidget.h"
#include "protocol.h"

class choose_proto_menu_object : public menu_object {
  public:
    choose_proto_menu_object(string name, QWidget* parent = 0) {
        this->parent = parent;
        this->name = name;
        this->menu = new chooseProtoWidget();
    }
    QWidget* getWidget() {
        return this->menu;
    }
    void reset() { 
        this->menu = new chooseProtoWidget();
    }
    void setWorkingDir(QDir dir) { }
    void leave() { }
    void changeProto(Protocol* proto) {}

  private:
    QWidget* parent;
    chooseProtoWidget* menu;
};
