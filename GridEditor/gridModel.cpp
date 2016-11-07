#include "gridModel.h"

using namespace std;

GridModel::GridModel(Grid* grid, QObject* parent) : QAbstractTableModel(parent) {
	if(grid) {
		this->grid = grid;
	} else {
		this->grid = new Grid();
	}
}
bool GridModel::setGrid(Grid* grid) {
	this->grid = grid;
	QAbstractItemModel::resetInternalData();
	return true;
}
int  GridModel::rowCount(const QModelIndex & parent) const {
	return this->grid->rowCount();
}
int  GridModel::columnCount(const QModelIndex & parent) const {
	return this->grid->columnCount();
}
QVariant  GridModel::data(const QModelIndex & index, int role) const {
	pair<int,int> p = make_pair(index.row(), index.column());
	Node* n = this->grid->findNode(p);
	if(n == NULL) {
		return QVariant();
	}
	return QString(n->cell->type);
}
bool GridModel::setData(const QModelIndex & index, const QVariant & value, int role) {
	pair<int,int> p = make_pair(index.row(), index.column());
	Node* n = this->grid->findNode(p);
	if(n == NULL) {
		return false;
	}
	Cell* cell = Cell::createCell(value.toString().toStdString());
	if(!cell) return false;
	n->cell = cell;
	return true;
}
Qt::ItemFlags GridModel::flags(const QModelIndex & index) const {
	if (!index.isValid())
		return Qt::ItemIsEnabled;

	return QAbstractItemModel::flags(index) | Qt::ItemIsEditable;
}
bool GridModel::insertRows(int row, int count, const QModelIndex & parent) {
	this->beginInsertRows(parent, row, row+count-1);
	this->grid->addRows(static_cast<unsigned int>(count), parent.column());
	this->endInsertRows();
	return true;
}
bool GridModel::removeRows(int row, int count, const QModelIndex & parent) {
	this->beginRemoveRows(parent, row, row+count-1);
	this->grid->removeRows(static_cast<unsigned int>(count), parent.column());
	this->endRemoveRows();
	return true;
}
bool GridModel::insertColumns(int column, int count, const QModelIndex & parent) {
	this->beginInsertColumns(parent, column, column+count-1);
	this->grid->addColumns(static_cast<unsigned int>(count), column);
	this->endInsertColumns();
	return true;
}
bool GridModel::removeColumns(int column, int count, const QModelIndex & parent) {
	this->beginRemoveColumns(parent, column, column+count-1);
	this->grid->removeColumns(static_cast<unsigned int>(count), column);
	this->endRemoveColumns();
	return true;
}
/*QModelIndex index(int row, int column, const QModelIndex & parent) const {
    QModelIndex ret = QTableModel(row, column, parent)
	return QAbstractItemModel::createIndex(parent.row(), parent.column())
}*/
