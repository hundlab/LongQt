#include "menu_object.h"
#include "runWidget.h"
#include "protocol.h"

class run_menu_object : public menu_object {
  public:
    run_menu_object(string name, Protocol* proto,QDir working_dir, QWidget* parent = 0) {
        this->name = name;
        this->parent = parent;
        this->proto = proto;
        this->working_dir = working_dir;
        this->menu = new runWidget(proto, working_dir);
    }
    QWidget* getWidget() {
        return menu;
    }
    void reset() {
//       this->menu = new (this->menu)  runWidget(proto, working_dir);
    }
    void setWorkingDir(QDir dir) {
        working_dir = dir;
        menu->setWorkingDir(dir);
    }
    void leave() {
    }
    void changeProto(Protocol* proto) {
        this->proto = proto;
        this->menu->setProto(proto);
    }
  private:
    runWidget* menu;
    QWidget* parent;
    Protocol* proto;
    QDir working_dir;
};
