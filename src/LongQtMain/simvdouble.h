#ifndef SIMVDOUBLE_H
#define SIMVDOUBLE_H

#include "simvar.h"

#include <QDoubleSpinBox>

class SimvDouble : public Simvar
{
public:
    explicit SimvDouble(shared_ptr<Protocol> proto, string name, QWidget *parent = 0);
    ~SimvDouble();
private:
    virtual void update_ui();
    void update_model(double value);

    QDoubleSpinBox* widg;
};

#endif // SIMVDOUBLE_H
