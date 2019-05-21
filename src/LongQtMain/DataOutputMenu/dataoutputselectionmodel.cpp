#include "dataoutputselectionmodel.h"

#include "guiUtils.h"

using namespace LongQt;

DataOutputSelectionModel::DataOutputSelectionModel(
    std::shared_ptr<Protocol> proto, QObject *parent)
    : QAbstractItemModel(parent) {
  this->proto = proto;

  auto vars = this->proto->cell()->vars();
  for (auto var : vars) {
    items.append(new DataOutputItem(proto, var));
  }
}

QModelIndex DataOutputSelectionModel::index(int row, int column,
                                            const QModelIndex &parent) const {
  if (parent.isValid()) {
    auto parentItem = static_cast<DataOutputItem *>(parent.internalPointer());
    auto child = parentItem->child(row);
    if (child) {
      return QAbstractItemModel::createIndex(row, column, child);
    } else {
      return QModelIndex();
    }
  } else {
    return QAbstractItemModel::createIndex(row, column, this->items[row]);
  }
}

QModelIndex DataOutputSelectionModel::parent(const QModelIndex &index) const {
  if (!index.isValid()) return QModelIndex();
  auto item = static_cast<DataOutputItem *>(index.internalPointer());
  auto parentItem = item->getParent();
  if (parentItem) {
    int idx = this->items.indexOf(parentItem);
    return this->index(idx, 0);
  } else {
    return QModelIndex();
  }
}

int DataOutputSelectionModel::rowCount(const QModelIndex &parent) const {
  if (!parent.isValid()) {
    return this->items.size();
  } else {
    auto item = static_cast<DataOutputItem *>(parent.internalPointer());
    return item->children().size();
  }
  return 0;
}

int DataOutputSelectionModel::columnCount(const QModelIndex &parent) const {
  return 4;
}

QVariant DataOutputSelectionModel::headerData(int section,
                                              Qt::Orientation orientation,
                                              int role) const {
  if (orientation == Qt::Vertical) return QVariant();

  if (role == Qt::DisplayRole) {
    switch (section) {
      case 0:
        return "Name";
      case 1:
        return "Trace";
      case 2:
        return "Measure";
      case 3:
        return "Type";
    }
  } else if (role == Qt::ToolTipRole) {
    switch (section) {
      case 0:
        return "Name of the variable";
      case 1:
        return "Save the trace of the variable. Begins saving at begin write "
               "time, and saves the value every write interval";
      case 2:
        return "Measure the variable. Saves the value of the measurement once "
               "per beat";
      case 3:
        return "Type of the variable";
    }
  }
  return QVariant();
}

QVariant DataOutputSelectionModel::data(const QModelIndex &index,
                                        int role) const {
  switch (role) {
    case Qt::DisplayRole:
      return this->dataDisplay(index);
    case Qt::CheckStateRole:
      return this->dataCheckState(index);
    case Qt::ToolTipRole:
      return this->dataToolTip(index);
    default:
      return QVariant();
  }
}

QVariant DataOutputSelectionModel::dataDisplay(const QModelIndex &index) const {
  auto item = static_cast<DataOutputItem *>(index.internalPointer());
  switch (index.column()) {
    case 0:
      return item->displayData();
    case 3:
      return item->getType();
    default:
      return QVariant();
  }
  return QVariant();
}

QVariant DataOutputSelectionModel::dataToolTip(const QModelIndex &index) const {
  auto item = static_cast<DataOutputItem *>(index.internalPointer());
  switch (index.column()) {
    case 0:
      return item->tooltipData();
    default:
      return QVariant();
  }
  return QVariant();
}

QVariant DataOutputSelectionModel::dataCheckState(
    const QModelIndex &index) const {
  auto item = static_cast<DataOutputItem *>(index.internalPointer());
  switch (index.column()) {
    case 1:
      if (!item->isMeasItem()) {
        return item->getData() ? 2 : 0;
      }
      break;
    case 2:
      if (item->isMeasItem()) {
        return item->getData() ? 2 : 0;
      }
      break;
    default:
      return QVariant();
  }
  return QVariant();
}

bool DataOutputSelectionModel::setData(const QModelIndex &index,
                                       const QVariant &value, int role) {
  if (role != Qt::CheckStateRole) return false;
  auto item = static_cast<DataOutputItem *>(index.internalPointer());
  item->setData(value.toBool());
  return true;
}

Qt::ItemFlags DataOutputSelectionModel::flags(const QModelIndex &index) const {
  auto flag = Qt::ItemIsSelectable | Qt::ItemIsEnabled;
  auto item = static_cast<DataOutputItem *>(index.internalPointer());
  if (!item->isMeasItem() && index.column() == 1 &&
      (item->varname == "vOld" || item->varname == "t")) {
    flag = flag & ~Qt::ItemIsEnabled;
  }

  if (index.column() == 1 && !item->isMeasItem()) {
    flag |= Qt::ItemIsUserCheckable;
  } else if (index.column() == 2 && item->isMeasItem()) {
    flag |= Qt::ItemIsUserCheckable;
  }
  return flag;
}

void DataOutputSelectionModel::changeProto(
    std::shared_ptr<LongQt::Protocol> proto) {
  this->proto = proto;
  this->reset();
}

void DataOutputSelectionModel::changeCell(std::shared_ptr<LongQt::Cell> cell) {
  this->reset();
}

void DataOutputSelectionModel::reset() {
  QAbstractItemModel::beginResetModel();
  this->items.clear();
  auto vars = this->proto->cell()->vars();
  for (auto var : vars) {
    items.append(new DataOutputItem(proto, var));
  }
  QAbstractItemModel::endResetModel();
}
