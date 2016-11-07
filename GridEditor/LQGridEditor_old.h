//GridEditor main window
#ifndef GRID_EDITOR_H
#define GRID_EDITOR_H

#include <QMainWindow>
#include <QDir>

#include "gridSettup.h"


namespace Ui {
	class GridEditor;
}

class GridEditor : public QWidget {
    Q_OBJECT
	public:
		GridEditor(QWidget* = 0);
		~GridEditor() {};
	private:
		Ui::GridEditor* ui;
	gridSetup* settup;
	gridProtocol* proto;
	QDir location;
};
#endif 
