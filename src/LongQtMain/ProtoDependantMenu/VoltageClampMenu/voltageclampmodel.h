#ifndef VOLTAGECLAMPMODEL_H
#define VOLTAGECLAMPMODEL_H
#include <QAbstractTableModel>
#include "voltageClampProtocol.h"
namespace LQ = LongQt;

class VoltageClampModel : public QAbstractTableModel {
  Q_OBJECT
 public:
  VoltageClampModel(std::shared_ptr<LQ::VoltageClamp> proto,
                    QObject* parent = 0);
  bool setProtocol(std::shared_ptr<LQ::VoltageClamp> proto);
  // implemented and reimplemented functions from QAbstractTableModel
  int rowCount(const QModelIndex& parent = QModelIndex()) const override;
  int columnCount(const QModelIndex& parent = QModelIndex()) const override;
  QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
  QVariant dataEdit(const QModelIndex& index) const;
  QVariant dataDisplay(const QModelIndex& index) const;
  QVariant dataToolTip(const QModelIndex& index) const;
  bool setData(const QModelIndex& index, const QVariant& value,
               int role = Qt::EditRole) override;
  Qt::ItemFlags flags(const QModelIndex& index) const override;
  // row & column positions are ignored for inserts but respected for removals
  bool insertRows(int row, int count,
                  const QModelIndex& parent = QModelIndex()) override;
  bool removeRows(int row, int count,
                  const QModelIndex& parent = QModelIndex()) override;
  bool insertColumns(int column, int count,
                     const QModelIndex& parent = QModelIndex()) override;
  bool removeColumns(int column, int count,
                     const QModelIndex& parent = QModelIndex()) override;
  QModelIndex index(int row, int column, const QModelIndex& parent) const override;
  QModelIndex parent(const QModelIndex& index) const override;
  QVariant headerData(int section, Qt::Orientation orientation,
                      int role = Qt::DisplayRole) const override;
  void reloadModel();
  void clear();

 private:
  std::shared_ptr<LQ::VoltageClamp> proto;
  QList<QString> colHeaders;
};

#endif  // VOLTAGECLAMPMODEL_H
