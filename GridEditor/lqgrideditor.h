/*
 * The main window for the LQGridEditor.pro
 * contains the other widgets which alow for configuring the grid
 */
#ifndef LQGRIDEDITOR_H
#define LQGRIDEDITOR_H

#include <QMainWindow>
#include <QTableView>
#include "conductivityeditor.h"
#include "ionchannelconfig.h"

namespace Ui {
class LQGridEditor;
}

class LQGridEditor : public QMainWindow
{
    Q_OBJECT

public:
    explicit LQGridEditor(QWidget *parent = 0);
    ~LQGridEditor();

private:
    Ui::LQGridEditor *ui;
	QTableView* gridView;
	ConductivityEditor* condEdit = 0; 
	IonChannelConfig* ionConfig = 0;
	GridProtocol* proto = 0;
	QString saveFile = "";
private slots:
	void on_actionNew_triggered();
	void on_actionOpen_triggered();
	void on_actionSave_triggered();
	void on_actionSave_As_triggered();
	void on_actionSet_Conductivities_triggered();
	void on_actionConfigure_Ion_Channels_triggered();
};

#endif // LQGRIDEDITOR_H
