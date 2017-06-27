#include "gridModel.h"
#include "cellutils.h"
#include "gridCell.h"
#include <QDebug>

using namespace std;

GridModel::GridModel(GridProtocol* proto, QObject* parent) : QAbstractTableModel(parent) {
    if(proto) {
        this->proto = proto;
    } else {
        this->proto = new GridProtocol();
    }
    this->grid = ((GridCell*)proto->cell)->getGrid();
    cellMap = CellUtils::cellMap;
    cellMap["Inexcitable Cell"] = [] () {return new Cell;};

}
bool GridModel::setProtocol(GridProtocol* proto) {
    this->proto = proto;
    this->grid = ((GridCell*)proto->cell)->getGrid();
    QAbstractItemModel::resetInternalData();
    return true;
}
int  GridModel::rowCount(const QModelIndex&) const {
    return this->grid->rowCount();
}
int  GridModel::columnCount(const QModelIndex&) const {
    return this->grid->columnCount();
}
QVariant GridModel::data(const QModelIndex& index, int role) const {
    switch(role) {
        case Qt::DisplayRole:
            return this->dataDisplay(index);
            break;
        case Qt::ToolTipRole:
            return this->dataToolTip(index);
            break;
        case Qt::StatusTipRole:
            return this->dataStatusTip(index);
            break;
        default:
            return QVariant();
    }
}
QVariant GridModel::dataDisplay(const QModelIndex & index) const {
    if(index.internalPointer() == 0) {
        Node* n = this->grid->operator ()(index.row(), index.column());
        if(n == NULL) {
            return QVariant();
        }
        return QString(n->cell->type);
    } else {
        pair<int,int> p = make_pair(index.parent().row(), index.parent().column());
        if(index.row() == 0) {
            if(index.column() == 0) {
                return this->proto->getStimNodes().count(p) > 0;
            } else if(index.column() == 1) {
                return this->proto->gridMeasureMgr()->dataNodes().count(p) > 0;
            }
        } else if(index.row() == 1) {
            switch(index.column()) {
                case 0: //top
                    return grid->fibery.at(p.second).B.at(p.first);
                    break;
                case 1: //right
                    return grid->fiber.at(p.first).B.at(p.second+1);
                    break;
                case 2: //bottom
                    return  grid->fibery.at(p.second).B.at(p.first+1);
                    break;
                case 3: //left
                    return  grid->fiber.at(p.first).B.at(p.second);
                    break;
            }
        }
    }
    return QVariant();
}
QVariant GridModel::dataToolTip(const QModelIndex & index) const {
    return this->data(index);
}
QVariant GridModel::dataStatusTip(const QModelIndex & index) const {
    QString statusTip = "";
    statusTip += "Top: "+QString::number(index.child(1,0).data().toDouble())+" ";
    statusTip += "Right: "+QString::number(index.child(1,1).data().toDouble())+" ";
    statusTip += "Bottom: "+QString::number(index.child(1,2).data().toDouble())+" ";
    statusTip += "Left: "+QString::number(index.child(1,3).data().toDouble())+" ";

    return statusTip;
}
bool GridModel::setData(const QModelIndex & index, const QVariant & value, int) {
    bool success = false;
    if(index.internalPointer() == 0) {
        CellInfo info;
        info.X = index.row();
        info.Y = index.column();
        try {
            info.cell = this->cellMap.at(value.toString().toStdString())();
            info.dx = *proto->cell->pars["dx"];
            info.dy = *proto->cell->pars["dy"];
            info.np = *proto->cell->pars["np"];
            if(*info.cell->pars["dtmin"] < *proto->cell->pars["dtmin"]) {
                *proto->cell->pars["dtmin"] = *info.cell->pars["dtmin"];
            }
            if(*info.cell->pars["dtmed"] < *proto->cell->pars["dtmed"]) {
                *proto->cell->pars["dtmed"] = *info.cell->pars["dtmed"];
            }
            if(*info.cell->pars["dtmax"] < *proto->cell->pars["dtmax"]) {
                *proto->cell->pars["dtmax"] = *info.cell->pars["dtmax"];
            }
            this->grid->setCellTypes(info);
            emit cellChanged(proto->cell);
            emit dataChanged(index, index);
        } catch(const std::out_of_range&) {
            qWarning("%s not a valid cell type or (%i,%i) out of range",qUtf8Printable(value.toString()),info.Y,info.X);
        }
        success = true;
    } else {
        pair<int,int> p = make_pair(index.parent().row(), index.parent().column());
        if(index.row() == 0) {
            if(index.column() == 0) {
                if(value.toBool()) {
                    this->proto->getStimNodes().insert(p);
                } else {
                    this->proto->getStimNodes().erase(p);
                }
                success = true;
            } else if(index.column() == 1) {
                if(value.toBool()) {
                    auto nodes = this->proto->gridMeasureMgr()->dataNodes();
                    nodes.insert(p);
                    this->proto->gridMeasureMgr()->dataNodes(nodes);
                } else {
                    auto nodes = this->proto->gridMeasureMgr()->dataNodes();
                    nodes.erase(p);
                    this->proto->gridMeasureMgr()->dataNodes(nodes);
                }
                success = true;
            }
        } else if(index.row() == 1) {
            CellInfo u;
            u.X = index.parent().row();
            u.Y = index.parent().column();
            u.dx = *proto->cell->pars["dx"];
            u.dy = *proto->cell->pars["dy"];
            u.np = *proto->cell->pars["np"];
            //this works because column is setup follow cellutils_side.h:Side
            double val = value.toDouble();
            if(this->percent) {
                val /= 100;
            }
            u.c[index.column()] = val;
            u.c_perc = this->percent;
            try {
                grid->setCellTypes(u);
                success = true;
            } catch(std::out_of_range) {
                qWarning("(%i,%i) out of range", u.Y,u.X);
                success = false;
            }
        }
        if(success) {
            emit dataChanged(index.parent(), index.parent());
        }
    }
    return success;
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
    //imput must not be within grid size
    //sanitize inputs
    if(row < 0) {
        row = 0;
    }
    if(row > this->rowCount()) {
        row = this->rowCount()-1;
    }
    int last = row+count-1;
    if(last >= this->rowCount()) {
        last = this->rowCount()-1;
    }
    this->beginRemoveRows(parent, row, last);
    this->grid->removeRows(static_cast<unsigned int>(count), row);
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
    //imput must not be within grid size
    //sanitize inputs
    if(column < 0) {
        column = 0;
    }
    if(column > this->columnCount()) {
        column = this->columnCount()-1;
    }
    int last = column+count-1;
    if(last >= this->columnCount()) {
        last = this->columnCount()-1;
    }
    this->beginRemoveColumns(parent, column, last);
    this->grid->removeColumns(static_cast<unsigned int>(count), column);
    this->endRemoveColumns();
    return true;
}
QModelIndex GridModel::index(int row, int column, const QModelIndex & parent) const {
    if(!parent.isValid()) {
        return QAbstractTableModel::index(row, column, parent);
    }
    //	if(row > 0 || column > 1) {
    //		return QModelIndex();
    //	}
    return QAbstractTableModel::createIndex(row, column, new QPair<int,int>(parent.row(), parent.column()));
}
QModelIndex GridModel::parent(const QModelIndex & index) const {
    QPair<int,int>* p = 0;
    if((p = static_cast<QPair<int,int>*>(index.internalPointer()))) {
        return this->index(p->first, p->second, QModelIndex());
    }
    return QModelIndex();
}

void GridModel::reloadModel() {
    this->beginResetModel();
    this->endResetModel();
}

void GridModel::clear() {
    this->grid->reset();
    this->beginResetModel();
    this->endResetModel();
}

bool GridModel::getPercent() {
    return this->percent;
}

void GridModel::setPercent(bool percent) {
    this->percent = percent;
}
