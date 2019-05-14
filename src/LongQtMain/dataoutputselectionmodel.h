#ifndef DATAOUTPUTSELECTIONMODEL_H
#define DATAOUTPUTSELECTIONMODEL_H

#include <QAbstractItemModel>
#include <memory>
#include "dataoutputitem.h"
#include "measurefactory.h"
#include "protocol.h"

namespace LQ = LongQt;

class DataOutputSelectionModel : public QAbstractItemModel {
 public:
  DataOutputSelectionModel(std::shared_ptr<LQ::Protocol> proto,
                           QObject *parent = nullptr);
  QModelIndex index(int row, int column,
                    const QModelIndex &parent = QModelIndex()) const override;
  QModelIndex parent(const QModelIndex &index) const override;
  int rowCount(const QModelIndex &parent = QModelIndex()) const override;
  int columnCount(const QModelIndex &parent = QModelIndex()) const override;
  QVariant headerData(int section, Qt::Orientation orientation,
                      int role = Qt::DisplayRole) const override;
  QVariant data(const QModelIndex &index,
                int role = Qt::DisplayRole) const override;

  QVariant dataDisplay(const QModelIndex &index) const;
  QVariant dataToolTip(const QModelIndex &index) const;
  QVariant dataCheckState(const QModelIndex &index) const;
  bool setData(const QModelIndex &index, const QVariant &value,
               int role = Qt::EditRole) override;
  Qt::ItemFlags flags(const QModelIndex &index) const override;

  void changeProto(std::shared_ptr<LQ::Protocol> proto);
  void changeCell(std::shared_ptr<LQ::Cell> cell);
  void reset();

 private:
  std::shared_ptr<LQ::Protocol> proto;
  QVector<DataOutputItem *> items;
};

#endif  // DATAOUTPUTSELECTIONMODEL_H
