//GridEditor class implementaions
#include <QStandardPaths>
#include "LQGridEditor.cpp"

GridEditor::GridEditor(QWidget* parent = 0) : QMainWindow(parent) {
	//construct class variables
	this->proto = new gridProtocol();
	this->location = QStandardPaths::standardLocations(QStandardPaths::DocumentsLocation).first();
	this->settup = new gridSetup(this->proto, this->location);
	this->
}
