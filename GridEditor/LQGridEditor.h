//GridEditor main window
#include <QMainWindow>
#include <QDir>

#include "gridSettup.h"

#ifndef GRID_EDITOR_H
#define GRID_EDITOR_H

class GridEditor : public QMainWindow {
    Q_OBJECT
	public:
		GridEditor(QWidget* = 0);
		~GridEditor() {};
	private:
	gridSetup* settup;
	gridProtocol* proto;
	QDir location;
};
#endif 
