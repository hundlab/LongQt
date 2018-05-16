#include "simvdir.h"

#include <QFileDialog>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QPushButton>
#include <protocol.h>

SimvDir::SimvDir(shared_ptr<Protocol> proto, string name, QWidget *parent) :
    Simvar(proto,name,parent)
{
    line = new QLineEdit();
    QPushButton* setDir = new QPushButton("Choose");
    QHBoxLayout* layout = new QHBoxLayout(this);
    layout->setContentsMargins(0,0,0,0);
    layout->addWidget(line);
    layout->addWidget(setDir);
    connect(line, &QLineEdit::textEdited, this, &SimvDir::update_model);
    connect(setDir, &QPushButton::clicked, [=] () {
        QString value = QFileDialog::getExistingDirectory(this,QString(name.c_str()),proto->pars[this->name].get().c_str());
        if(value != "") {
            this->update_model(value);
            this->update_ui();
        }
    });
}

void SimvDir::update_ui() {
    QString model_line = QString(proto->pars[this->name].get().c_str());
    if(line->text() != model_line) {
        line->setText(model_line);
    }
    emit updated();
}

void SimvDir::update_model(QString value) {
    proto->pars.at(name).set(value.toStdString());
}

SimvDir::~SimvDir()
{}
