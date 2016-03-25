#include "menu_object.h"

class run_menu_object : public menu_object {
  public:
    run_menu_object(string name, QWidget* parent = 0) {
        this->name = name;
        this->parent = parent;
        this->menu = new_widget();
    }
    QWidget* getWidget() {
        return menu;
    }
    void reset() {
       this->menu = new_widget();
    }
    void setWorkingDir(QDir dir) {}
    void leave() {
        menu->write_file();
    }

  private:
    QWidget* menu;
    QWidget* parent;
    QPushButton* run_button;
    QProgressBar* pdialog;
    QWidget* new_widget() {
        QWidget* run_button_container = new QWidget(parent);
        QGridLayout* run_button_container_layout = new QGridLayout();
        QPushButton* run_button = new QPushButton("Run Simulations");
        run_button_container_layout->addWidget(run_button, 0,0,1,2);
        run_button_container_layout->addWidget(new QLabel("Progress"), 1,0);
        run_button_container_layout->addWidget(pdialog, 1,1);
        run_button_container->setLayout(run_button_container_layout);

        connect(run_button, SIGNAL(clicked()),this,SLOT(run_sims()));
        return run_button_container;
    }
};
