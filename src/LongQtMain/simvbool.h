#ifndef SIMVBOOL_H
#define SIMVBOOL_H

#include "simvar.h"

#include <QCheckBox>

class SimvBool : public Simvar
{
public:
    explicit SimvBool(shared_ptr<Protocol> proto, string name, QWidget *parent = 0);
    ~SimvBool();
private:
    virtual void update_ui();
    void update_model(int value);

    QCheckBox* widg;
};

#endif // SimvBool_H
