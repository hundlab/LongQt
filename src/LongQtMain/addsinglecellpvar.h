#ifndef ADDSINGLECELLPVAR_H
#define ADDSINGLECELLPVAR_H

#include <QMap>
#include <QString>
#include <QDialog>
#include <pvarscell.h>
#include "protocol.h"
namespace LQ = LongQt;

namespace Ui {
    class AddSingleCellPvar;
}

class AddSingleCellPvar : public QDialog
{
    Q_OBJECT

    public:
        explicit AddSingleCellPvar(std::shared_ptr<LQ::Protocol> proto, std::pair<QString, LQ::PvarsCell::IonChanParam*> parampair, QWidget *parent = 0);
        ~AddSingleCellPvar();

private:
        void updateIonChannelType();
        void setCurrentSelect(QString name, LQ::PvarsCell::IonChanParam *param);

        Ui::AddSingleCellPvar *ui;
        std::shared_ptr<LQ::Protocol> proto = 0;
        QMap<QString,QString> pvarsDescriptions;

    public slots:
        void changeProto(std::shared_ptr<LQ::Protocol> proto);
        void changeCell(std::shared_ptr<LQ::Cell> cell);

    signals:
        void pvarsChanged();

    private slots:
        void on_randomize_stateChanged(int state);
        void on_normalDist_toggled(bool checked);
        void on_addButton_clicked();
};

#endif // ADDSINGLECELLPVAR_H
