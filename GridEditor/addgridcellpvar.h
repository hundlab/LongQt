#ifndef ADDGRIDCELLPVAR_H
#define ADDGRIDCELLPVAR_H

#include <QDialog>
#include <QMap>
#include <QString>
#include <QTableView>
#include <QPointer>
#include "gridProtocol.h"

namespace Ui {
    class AddGridCellPvar;
}

class AddGridCellPvar : public QDialog
{
    Q_OBJECT

    public:
        explicit AddGridCellPvar(QTableView* view, shared_ptr<GridProtocol> proto, QWidget *parent = 0);
        ~AddGridCellPvar();

    private:
        void updateIonChannelType();
        set<pair<int,int>> getInitial();

        shared_ptr<GridProtocol> proto;
        QMap<QString,QString> pvarsDescriptions;
        Ui::AddGridCellPvar *ui;
        QPointer<QTableView> view = 0;

        public slots:
            void changeProto(shared_ptr<GridProtocol> proto);
        void changeCell(shared_ptr<Cell> cell);

signals:
        void pvarsChanged();

        private slots:
            void on_randomize_stateChanged(int state);
        void on_multiple_stateChanged(int state);
        void on_normalDist_toggled(bool checked);
        void on_addButton_clicked();
};

#endif // ADDGRIDCELLPVAR_H
