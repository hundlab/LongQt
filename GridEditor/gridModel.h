#include <QAbstractTableModel>
#include "grid.h"

class GridModel : public QAbstractTableModel {
	public:
		GridModel(Grid* grid = 0, QObject* parent = 0);
		bool setGrid(Grid* grid);
		//implemented and reimplemented functions from QAbstractTableModel
		int rowCount(const QModelIndex & parent = QModelIndex()) const;
		int columnCount(const QModelIndex & parent = QModelIndex()) const;
		QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;
		bool setData(const QModelIndex & index, const QVariant & value, int role = Qt::EditRole);
		Qt::ItemFlags flags(const QModelIndex & index) const;
		bool insertRows(int row, int count, const QModelIndex & parent = QModelIndex());
		bool removeRows(int row, int count, const QModelIndex & parent = QModelIndex());
		bool insertColumns(int column, int count, const QModelIndex & parent = QModelIndex());
		bool removeColumns(int column, int count, const QModelIndex & parent = QModelIndex());
	private:
		Grid* grid;
};
