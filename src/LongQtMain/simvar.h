#ifndef SIMVAR_H
#define SIMVAR_H

#include <protocol.h>
#include <QWidget>

class Simvar: public QWidget
{
    Q_OBJECT
public:
    explicit Simvar(shared_ptr<Protocol> proto, string name, QWidget *parent = 0);
    ~Simvar();

    virtual void update_ui() = 0;
protected:
    shared_ptr<Protocol> proto;
    string name;
public slots:
    virtual void changeProto(shared_ptr<Protocol> proto);
    virtual void changeCell(shared_ptr<Cell>);
signals:
    void cellChanged(shared_ptr<Cell>);
    void updated();
};

#endif // SIMVAR_H
