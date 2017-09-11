/*
 * A widget for setting ion channels in a grid (or single cell)
 * ion channels are the *Factor in cell->pars
 */
#ifndef IONCHANNELCONFIG_H
#define IONCHANNELCONFIG_H

#include <QWidget>
#include <QTableView>
#include <QPointer>

#include "gridProtocol.h"
#include "gridModel.h"
#include "addgridcellpvar.h"

namespace Ui {
    class PvarMenu;
}

class IonChannelConfig : public QWidget
{
    Q_OBJECT

    public:
        explicit IonChannelConfig(QTableView* view, shared_ptr<GridProtocol> proto, QWidget *parent = 0);
        ~IonChannelConfig();
        void updateList();

    private:
        Ui::PvarMenu *ui;
        GridModel* model = 0;
        shared_ptr<GridProtocol> proto = 0;
        QTableView* view = 0;
        QPointer<AddGridCellPvar> addmenu;


    public slots:
        void changeCell(shared_ptr<Cell>);
        void changeProto(shared_ptr<GridProtocol> proto);


    private slots:
        void on_actionDelete_triggered();
        void on_actionShow_Cells_triggered();
        void on_addButton_triggered();
        void on_removeButton_triggered();
        void on_infoButton_triggered();
        void on_refreshButton_triggered();
};

#endif // IONCHANNELCONFIG_H
