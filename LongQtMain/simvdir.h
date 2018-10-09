#ifndef SIMVDIR_H
#define SIMVDIR_H

#include "simvar.h"

#include <QLineEdit>
#include <QWidget>

class SimvDir : public Simvar
{
public:
    explicit SimvDir(shared_ptr<Protocol> proto, string name, QWidget *parent = 0);
    ~SimvDir();
private:
    virtual void update_ui();
    void update_model(QString value);

     QLineEdit* line;
};

#endif // SimvDir_H
