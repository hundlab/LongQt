#include "voltageclampmodel.h"
#include <logger.h>
#include <utility>
#include <vector>
namespace LQ = LongQt;

VoltageClampModel::VoltageClampModel(std::shared_ptr<LQ::VoltageClamp> proto,
                                     QObject *parent)
    : QAbstractTableModel(parent) {
  this->proto = proto;
  this->colHeaders = {"Time (ms)", "Voltage (mV)"};
}

bool VoltageClampModel::setProtocol(std::shared_ptr<LQ::VoltageClamp> proto) {
  this->proto = proto;
  QAbstractItemModel::resetInternalData();
  return true;
}

int VoltageClampModel::rowCount(const QModelIndex &parent) const {
  return this->proto->clamps().size();
}

int VoltageClampModel::columnCount(const QModelIndex &parent) const {
  return 2;
}

QVariant VoltageClampModel::data(const QModelIndex &index, int role) const {
  switch (role) {
    case Qt::EditRole:
      return this->dataEdit(index);
    case Qt::DisplayRole:
      return this->dataDisplay(index);
    case Qt::ToolTipRole:
      return this->dataToolTip(index);
    default:
      return QVariant();
  }
}

QVariant VoltageClampModel::dataEdit(const QModelIndex &index) const {
  try {
    auto tv = this->proto->clamps().at(index.row());
    return index.column() == 0 ? tv.first : tv.second;
  } catch (std::out_of_range) {
    LQ::Logger::getInstance()->write(
        "VoltageClampModel: requested data is out of range");
  }
  return QVariant();
}

QVariant VoltageClampModel::dataDisplay(const QModelIndex &index) const {
  try {
    auto tv = this->proto->clamps().at(index.row());
    if (index.column() == 0) {
      return QString::number(tv.first) + " ms";
    } else {
      return QString::number(tv.second) + " mV";
    }
  } catch (std::out_of_range) {
    LQ::Logger::getInstance()->write(
        "VoltageClampModel: requested data is out of range");
  }
    return QVariant();
}

QVariant VoltageClampModel::dataToolTip(const QModelIndex &index) const
{
  if (index.column() == 0) {
    return "Start time for voltage";
  } else {
    return "Voltage to be applied to the cell";
  }
}

bool VoltageClampModel::setData(const QModelIndex &index, const QVariant &value,
                                int role) {
  try {
    auto tv = this->proto->clamps().at(index.row());
    if (index.column() == 0) {
      this->proto->removeClamp(index.row());
      int loc = this->proto->insertClamp(value.toDouble(), tv.second);
      this->beginMoveRows(index.parent(), index.row(), index.row(), index.parent(), loc+1);
      this->endMoveRows();
    } else if (index.column() == 1) {
      this->proto->changeClampVoltage(index.row(), value.toDouble());
    }
    return true;
  } catch (std::out_of_range) {
    LQ::Logger::getInstance()->write(
        "VoltageClampModel: requested data is out of range");
  }
  return false;
}

Qt::ItemFlags VoltageClampModel::flags(const QModelIndex &index) const {
  if (!index.isValid()) return Qt::ItemIsEnabled;

  return QAbstractItemModel::flags(index) | Qt::ItemIsEditable;
}

bool VoltageClampModel::insertRows(int row, int count,
                                   const QModelIndex &parent) {
  this->beginInsertRows(parent, row, row + count - 1);
  for (int i = 0; i < count; ++i) {
    this->proto->insertClamp(0, 0);
  }
  this->endInsertRows();
  return true;
}

bool VoltageClampModel::removeRows(int row, int count,
                                   const QModelIndex &parent) {
  this->beginRemoveRows(parent, row, row + count - 1);
  for (int i = 0; i < count; ++i) {
    this->proto->removeClamp(row);
  }
  this->endRemoveRows();
  return true;
}

bool VoltageClampModel::insertColumns(int column, int count,
                                      const QModelIndex &parent) {
  return false;
}

bool VoltageClampModel::removeColumns(int column, int count,
                                      const QModelIndex &parent) {
  return false;
}

QModelIndex VoltageClampModel::index(int row, int column,
                                     const QModelIndex &parent) const {
  return QAbstractTableModel::index(row, column, parent);
}

QModelIndex VoltageClampModel::parent(const QModelIndex &index) const {
  return QModelIndex();
}

QVariant VoltageClampModel::headerData(int section, Qt::Orientation orientation,
                                       int role) const {
  if (role == Qt::DisplayRole) {
    if (orientation == Qt::Horizontal) {
      return QString(this->colHeaders[section]);
    } else {
      return QString::number(section + 1);
    }
  }
  return QVariant();
}

void VoltageClampModel::reloadModel() {
  this->beginResetModel();
  this->endResetModel();
}

void VoltageClampModel::clear() {
  this->proto->clamps(std::vector<std::pair<double, double>>());
}
