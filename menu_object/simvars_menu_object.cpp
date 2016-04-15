#include "menu_object.h"
#include "varmenu.h"

class simvars_menu_object : public menu_object {
  public:
    simvars_menu_object(string name, Protocol* proto, QDir working_dir, QWidget* parent = 0) {
        this->proto = proto;
        this->name = name;
        this->working_dir = working_dir;
        this->parent = parent;
        this->menu = new simvarMenu(proto, working_dir, parent);
    }
    QWidget* getWidget() {
        return menu;
    }
    void reset() {
        this->menu->reset();
    }
    void setWorkingDir(QDir dir) {
        working_dir = dir;
    }
    void leave() {
        menu->write_file();
    }
    void changeProto(Protocol* proto) {
        this->menu->changeProto(proto);
    }
  private:
    simvarMenu* menu;
    Protocol* proto;
    QDir working_dir;
    QWidget* parent;
};
