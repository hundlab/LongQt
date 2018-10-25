#ifndef SIMVBOOL_H
#define SIMVBOOL_H

#include "simvar.h"

#include <QCheckBox>
namespace LQ = LongQt;

class SimvBool : public Simvar
{
public:
    explicit SimvBool(std::shared_ptr<LQ::Protocol> proto, std::string name, QWidget *parent = 0);
    ~SimvBool();
private:
    virtual void update_ui();
    void update_model(int value);

    QCheckBox* widg;
};

#endif // SimvBool_H
