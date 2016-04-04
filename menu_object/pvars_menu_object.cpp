#include "menu_object.h"
#include "varmenu.h"

class pvars_menu_object : public menu_object {
  public:
        pvars_menu_object(string name, Protocol* proto, QDir working_dir, QWidget* parent = 0) {
        this->name = name;
        this->proto = proto;
        this->working_dir = working_dir;
        this->parent = parent;
        this->menu = new pvarMenu(proto, working_dir, parent);
    }
    QWidget* getWidget() {
        return menu;
    }
    void reset() {
       this->menu = new pvarMenu(proto, working_dir, parent);
    }
    void setWorkingDir(QDir dir) {
        working_dir = dir;
        menu->setWorkingDir(dir);
    }
    void leave() {
        menu->write_file();
    }
    void changeProto(Protocol* proto) {
        this->proto = proto;
    }
  private:
    pvarMenu* menu;
    Protocol* proto;
    QDir working_dir;
    QWidget* parent;
};
