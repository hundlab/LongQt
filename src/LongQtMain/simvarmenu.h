#ifndef SIMVARMENU_H
#define SIMVARMENU_H

#include <simvar.h>

#include <QWidget>

#include <protocol.h>
#include <gridsetupwidget.h>
#include <QFormLayout>

namespace Ui {
class SimvarMenu;
}

class SimvarMenu : public QWidget
{
    Q_OBJECT

public:
    explicit SimvarMenu(shared_ptr<Protocol> initial_proto, QWidget *parent = 0);
    void createMenu();
    ~SimvarMenu();

    void removeGrid();

private:
    typedef function<Simvar*(shared_ptr<Protocol>, string)> SimvarInitializer;

    Ui::SimvarMenu *ui;
    shared_ptr<Protocol> proto;
//Buttons & their labels
    QMap<QString, QString> descriptions;
    QMap<QString, Simvar*> simvars;
    GridSetupWidget* grid = 0;
//initalizer
    void initialize(const map<string,GetSetRef>::iterator it);
//screen functions
    void update_menu(); //make menu match pars
    bool signalCellTypeChange = true;

public slots:
    void changeProto(shared_ptr<Protocol> proto);
    void changeCell(shared_ptr<Cell>);
    void reset();
    void setWorkingDir(QDir& dir);
signals:
    void cellChanged(shared_ptr<Cell>);
    void updated();
};

#endif // SIMVARMENU_H
