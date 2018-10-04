#ifndef SIMVINT_H
#define SIMVINT_H

#include "simvar.h"

#include <QSpinBox>

class SimvInt : public Simvar
{
public:
    explicit SimvInt(shared_ptr<Protocol> proto, string name, QWidget *parent = 0);
    ~SimvInt();
private:
    virtual void update_ui();
    void update_model(int value);

    QSpinBox* widg;
};

#endif // SIMVINT_H
