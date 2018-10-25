#ifndef SIMVDIR_H
#define SIMVDIR_H

#include "simvar.h"

#include <QLineEdit>
#include <QWidget>
namespace LQ = LongQt;

class SimvDir : public Simvar
{
public:
    explicit SimvDir(std::shared_ptr<LQ::Protocol> proto, std::string name, QWidget *parent = 0);
    ~SimvDir();
private:
    virtual void update_ui();
    void update_model(QString value);

     QLineEdit* line;
};

#endif // SimvDir_H
