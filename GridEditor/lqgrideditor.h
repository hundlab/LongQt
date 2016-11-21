#ifndef LQGRIDEDITOR_H
#define LQGRIDEDITOR_H

#include <QMainWindow>
#include <QTableView>
#include "conductivityeditor.h"

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
private slots:
	void on_actionSet_Conductivities_triggered();
};

#endif // LQGRIDEDITOR_H
