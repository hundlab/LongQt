#ifndef SIMVDOUBLE_H
#define SIMVDOUBLE_H

#include "simvar.h"

#include <QDoubleSpinBox>
namespace LQ = LongQt;

class SimvDouble : public Simvar
{
public:
    explicit SimvDouble(std::shared_ptr<LQ::Protocol> proto, std::string name, QWidget *parent = 0);
    ~SimvDouble();
private:
    virtual void update_ui();
    void update_model(double value);

    QDoubleSpinBox* widg;
};

#endif // SIMVDOUBLE_H
