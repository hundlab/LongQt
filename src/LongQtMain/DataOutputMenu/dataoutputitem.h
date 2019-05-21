#ifndef DATAOUTPUTITEM_H
#define DATAOUTPUTITEM_H

#include <QString>
#include <QVector>
#include <memory>
#include <string>

#include "protocol.h"

namespace LQ = LongQt;

class DataOutputItem {
 public:
  DataOutputItem(std::shared_ptr<LQ::Protocol> proto, std::string varname);
  DataOutputItem(std::shared_ptr<LQ::Protocol> proto, std::string varname,
                 std::string measname, DataOutputItem* parent);

  ~DataOutputItem();

  DataOutputItem* getParent() const;
  QVector<DataOutputItem*> children() const;
  DataOutputItem* child(int pos) const;

  QString displayData() const;
  QString tooltipData() const;
  void setData(bool set);
  bool getData() const;
  QString getType() const;
  bool isMeasItem() const;
  std::string varname = "";
  std::string measname = "";

 private:
  std::shared_ptr<LQ::Protocol> proto;
  QVector<DataOutputItem*> __children;
  DataOutputItem* __parent = nullptr;

  static QMap<QString, QMap<QString, QString>> dvarsDescriptions;
  static QMap<QString, QMap<QString, QString>> measDescriptions;
};

#endif  // DATAOUTPUTITEM_H
