#ifndef GRID_MODEL_H
#define GRID_MODEL_H
#include <QAbstractTableModel>
#include "grid.h"
#include "cellUtils.h"
#include "gridProtocol.h"

class GridModel : public QAbstractTableModel {
	Q_OBJECT
	public:
		GridModel(gridProtocol* grid = 0, QObject* parent = 0);
		bool setProtocol(gridProtocol* grid);
		//implemented and reimplemented functions from QAbstractTableModel
		int rowCount(const QModelIndex & parent = QModelIndex()) const;
		int columnCount(const QModelIndex & parent = QModelIndex()) const;
		QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;
		QVariant dataDisplay(const QModelIndex & index) const;
		QVariant dataToolTip(const QModelIndex & index) const;
		QVariant dataStatusTip(const QModelIndex & index) const;
		bool setData(const QModelIndex & index, const QVariant & value, int role = Qt::EditRole);
		Qt::ItemFlags flags(const QModelIndex & index) const;
		bool insertRows(int row, int count, const QModelIndex & parent = QModelIndex());
		bool removeRows(int row, int count, const QModelIndex & parent = QModelIndex());
		bool insertColumns(int column, int count, const QModelIndex & parent = QModelIndex());
		bool removeColumns(int column, int count, const QModelIndex & parent = QModelIndex());
		QModelIndex index(int row, int column, const QModelIndex & parent) const;
		QModelIndex parent(const QModelIndex & index) const;
		void reloadModel();
		void clear();
	signals:
   		void cell_type_changed();
	private:
		gridProtocol* proto;
		Grid* grid;
		map<string, CellInitializer> cellMap;
};
#endif
