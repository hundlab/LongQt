#include "menu_object.h"

class graph_menu_object : public menu_object {
  public:
    graph_menu_object(string name, Protocol* proto, QDir working_dir, QWidget* parent = 0) {
        this->proto = proto;
        this->name = name;
        this->working_dir = working_dir;
        this->parent = parent;
        this->menu = new Dialog(proto, working_dir, parent);
    }
    QWidget* getWidget() {
        return menu;
    }
    void reset() {
       this->menu = new Dialog(proto, working_dir, parent);
    }
    void setWorkingDir(QDir dir) {
        working_dir = dir;
    }
    void leave() {
    }
    void changeProto(Protocol* proto) {
        this->proto = proto;
    }
  private:
    Dialog* menu;
    Protocol* proto;
    QDir working_dir;
    QWidget* parent;
};
