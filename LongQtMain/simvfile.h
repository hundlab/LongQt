#ifndef SIMVFILE_H
#define SIMVFILE_H

#include "simvar.h"

#include <QLineEdit>

class SimvFile : public Simvar
{
public:
    explicit SimvFile(shared_ptr<Protocol> proto, string name, QWidget *parent = 0);
    ~SimvFile();
private:
    virtual void update_ui();
    void update_model(QString value);

    QLineEdit* widg;
};

#endif // SimvFile_H
