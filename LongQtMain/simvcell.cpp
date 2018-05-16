#include "oneitemlayout.h"
#include "simvcell.h"

#include <protocol.h>

SimvCell::SimvCell(shared_ptr<Protocol> proto, string name, QWidget *parent) :
    Simvar(proto,name,parent)
{
    this->widg = new QComboBox();
    auto layout = new OneItemLayout(this);
    layout->addWidget(widg);
    QStringList cell_options;
    for(auto& im :proto->cellOptions()) {
        cell_options << im.c_str();
    }
    widg->addItems(cell_options);
    connect(widg, QOverload<const QString&>::of(&QComboBox::currentIndexChanged), this, &SimvCell::update_model);
}

void SimvCell::update_ui() {
    int index = widg->findText(proto->pars[this->name].get().c_str());
    this->change_model = false;
    if(index != -1) {
//        if(!this->signalCellTypeChange) {
//            bool oldState = this->blockSignals(true);
//            this->setCurrentIndex(index);
//            this->blockSignals(oldState);
//        } else {
          widg->setCurrentIndex(index);
//        }
    }
    this->change_model = true;

    emit updated();
}

void SimvCell::update_model(QString value) {
    if(!this->change_model) return;
    proto->pars.at(name).set(value.toStdString());
    CellUtils::set_default_vals(*this->proto);
    emit cellChanged(this->proto->cell());
}

SimvCell::~SimvCell()
{}
