#ifndef SIMVCELLOPTS_H
#define SIMVCELLOPTS_H

#include "simvar.h"

#include <QCheckBox>
#include <QGroupBox>
#include <QMap>

class SimvCellOpts : public Simvar
{
public:
    explicit SimvCellOpts(shared_ptr<Protocol> proto, string name, QWidget *parent = 0);
    ~SimvCellOpts();
    void createMenu();
private:
    virtual void update_ui();
    void update_model(bool value);

    QMap<string,QCheckBox*> checkMap;
    QGroupBox* widg;
public slots:
    virtual void changeCell(shared_ptr<Cell>);
};

#endif // SimvCellOpts_H
