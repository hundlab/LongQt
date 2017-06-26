/*
 * The single cell equivelant of ion channel channel config
 * allows for cell->pars ending in Factor to be modified
 */
#ifndef PVARMENU_H
#define PVARMENU_H

#include <QWidget>
#include <QMap>
#include <QString>
#include <QPointer>
#include "protocol.h"
#include "grid.h"
#include "addsinglecellpvar.h"

using namespace std;

namespace Ui {
    class PvarMenu;
}

class PvarMenu : public QWidget
{
    Q_OBJECT

    public:
        explicit PvarMenu(Protocol* proto, QWidget *parent = 0);
        ~PvarMenu();
    private:
        void updateList();
        Ui::PvarMenu *ui;
        Protocol* proto = 0;
        QPointer<AddSingleCellPvar> addmenu;


    public slots:
        void changeProto(Protocol* proto);
        void changeCell(Cell* cell);

    private slots:
        void on_actionDelete_triggered();
        void on_actionShow_Cells_triggered();

        void on_addButton_triggered();
        void on_removeButton_triggered();
        void on_infoButton_triggered();
        void on_refreshButton_triggered();
};
#endif
