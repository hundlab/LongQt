/*
 * The single cell equivelant of ion channel channel config
 * allows for cell->pars ending in Factor to be modified
 */
#ifndef PVARMENU_H
#define PVARMENU_H

#include <QWidget>
#include <QMap>
#include <QString>
#include "protocol.h"
#include "grid.h"

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
	void updateIonChannelType();
	void updateList();
    Ui::PvarMenu *ui;
	Protocol* proto = 0;
    QMap<QString,QString> pvarsDescriptions;

public slots:
	void changeProto(Protocol* proto);
	void changeCell(Cell* cell);

private slots:
	void on_randomize_stateChanged(int state);
	void on_normalDist_toggled(bool checked);
	void on_addButton_clicked();
    void on_actionDelete_triggered();
    void on_actionShow_Cells_triggered();
};
#endif
