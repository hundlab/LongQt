#ifndef ADDSINGLECELLPVAR_H
#define ADDSINGLECELLPVAR_H

#include <QMap>
#include <QString>
#include <QDialog>
#include "protocol.h"

namespace Ui {
    class AddSingleCellPvar;
}

class AddSingleCellPvar : public QDialog
{
    Q_OBJECT

    public:
        explicit AddSingleCellPvar(Protocol* proto, QWidget *parent = 0);
        ~AddSingleCellPvar();

    private:
        void updateIonChannelType();

        Ui::AddSingleCellPvar *ui;
        Protocol* proto = 0;
        QMap<QString,QString> pvarsDescriptions;

    public slots:
        void changeProto(Protocol* proto);
        void changeCell(Cell* cell);

    signals:
        void pvarsChanged();

    private slots:
        void on_randomize_stateChanged(int state);
        void on_normalDist_toggled(bool checked);
        void on_addButton_clicked();
};

#endif // ADDSINGLECELLPVAR_H
