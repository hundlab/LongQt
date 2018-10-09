/*
 * GridModel acts as an interface between the Grid & Grid Protocol and the GUI
 */
#ifndef GRID_MODEL_H
#define GRID_MODEL_H
#include <QAbstractTableModel>
#include <QSet>
#include <QPair>
#include "grid.h"
#include "gridProtocol.h"
#include "cellutils.h"

class GridModel : public QAbstractTableModel {
	Q_OBJECT
	public:
		GridModel(shared_ptr<GridProtocol> grid = 0, QObject* parent = 0);
		bool setProtocol(shared_ptr<GridProtocol> grid);
		//implemented and reimplemented functions from QAbstractTableModel
		int rowCount(const QModelIndex & parent = QModelIndex()) const;
		int columnCount(const QModelIndex & parent = QModelIndex()) const;
		QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;
		QVariant dataDisplay(const QModelIndex & index) const;
		QVariant dataToolTip(const QModelIndex & index) const;
		QVariant dataStatusTip(const QModelIndex & index) const;
		bool setData(const QModelIndex & index, const QVariant & value, int role = Qt::EditRole);
		Qt::ItemFlags flags(const QModelIndex & index) const;
        //row & column positions are ignored for inserts but respected for removals
		bool insertRows(int row, int count, const QModelIndex & parent = QModelIndex());
		bool removeRows(int row, int count, const QModelIndex & parent = QModelIndex());
		bool insertColumns(int column, int count, const QModelIndex & parent = QModelIndex());
		bool removeColumns(int column, int count, const QModelIndex & parent = QModelIndex());
		QModelIndex index(int row, int column, const QModelIndex & parent) const;
		QModelIndex parent(const QModelIndex & index) const;
        QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
		void reloadModel();
		void clear();
		bool getPercent();
		void setPercent(bool percent);

	signals:
        void cellChanged(shared_ptr<Cell>);
	private:
		shared_ptr<GridProtocol> proto;
		Grid* grid;
        map<string, CellUtils::CellInitializer> cellMap;
		bool percent = false;
};
#endif
