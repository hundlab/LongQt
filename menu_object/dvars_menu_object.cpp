#include "menu_object.h"
#include "varmenu.h"

class dvars_menu_object : public menu_object {
  public:
    dvars_menu_object(string name, Protocol* proto, QDir working_dir, QWidget* parent = 0) {
        this->proto = proto;
        this->name = name;
        this->working_dir = working_dir;
        this->parent = parent;
        this->menu = new dvarMenu(proto, working_dir, parent);
    }
    QWidget* getWidget() {
        return menu;
    }
    void reset() {
       this->menu = new dvarMenu(proto, working_dir, parent);
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
    dvarMenu* menu;
    Protocol* proto;
    QDir working_dir;
    QWidget* parent;
};
