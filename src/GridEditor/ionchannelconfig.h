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
namespace LQ = LongQt;

namespace Ui {
    class PvarMenu;
}

class IonChannelConfig : public QWidget
{
    Q_OBJECT

    public:
        explicit IonChannelConfig(QTableView* view, std::shared_ptr<LQ::GridProtocol> proto, QWidget *parent = 0);
        ~IonChannelConfig();
        void updateList();

    private:
        Ui::PvarMenu *ui;
        GridModel* model = 0;
        std::shared_ptr<LQ::GridProtocol> proto = 0;
        QTableView* view = 0;
        QPointer<AddGridCellPvar> addmenu;


    public slots:
        void changeCell(std::shared_ptr<LQ::Cell>);
        void changeProto(std::shared_ptr<LQ::GridProtocol> proto);


    private slots:
        void on_actionDelete_triggered();
        void on_actionShow_Cells_triggered();
        void on_addButton_triggered();
        void on_removeButton_triggered();
        void on_infoButton_triggered();
        void on_refreshButton_triggered();
};

#endif // IONCHANNELCONFIG_H
