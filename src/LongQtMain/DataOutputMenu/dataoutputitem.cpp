#include "dataoutputitem.h"
#include "guiUtils.h"
#include "measurefactory.h"

#include <QMap>
#include <QRegExp>

namespace LQ = LongQt;

QMap<QString, QMap<QString, QString>> DataOutputItem::dvarsDescriptions;
QMap<QString, QMap<QString, QString>> DataOutputItem::measDescriptions;

DataOutputItem::DataOutputItem(std::shared_ptr<LongQt::Protocol> proto,
                               std::string varname) {
  this->proto = proto;
  this->varname = varname;
  this->__parent = nullptr;
  auto &mf = this->proto->measureMgr().measMaker;
  auto measType = mf.measureType(varname);
  auto measures = mf.measureOptions(measType);
  for (auto &measure : measures) {
    this->__children.append(new DataOutputItem(proto, varname, measure, this));
  }
  if (this->dvarsDescriptions.isEmpty()) {
    this->dvarsDescriptions =
        GuiUtils::readDesc(":/hoverText/cellVarsDesc.json");
  }
  if (this->measDescriptions.isEmpty()) {
    this->measDescriptions = GuiUtils::readDesc(":/hoverText/measDesc.json");
  }
}

DataOutputItem::DataOutputItem(std::shared_ptr<LongQt::Protocol> proto,
                               std::string varname, std::string measname,
                               DataOutputItem *parent) {
  this->proto = proto;
  this->varname = varname;
  this->__parent = parent;
  this->measname = measname;
}

DataOutputItem::~DataOutputItem() {
  for (auto child : this->__children) {
    delete child;
  }
}

DataOutputItem *DataOutputItem::getParent() const { return this->__parent; }

QVector<DataOutputItem *> DataOutputItem::children() const {
  return this->__children;
}

DataOutputItem *DataOutputItem::child(int pos) const {
  if (pos < this->__children.size()) {
    return this->__children[pos];
  }
  return nullptr;
}

QString DataOutputItem::displayData() const {
  if (this->isMeasItem()) {
    return this->measDescriptions[this->measname.c_str()]["Name"];
  } else {
    return QString(this->varname.c_str());
  }
}

QString DataOutputItem::tooltipData() const {
  if (this->isMeasItem()) {
    QString desc =
        this->measDescriptions[this->measname.c_str()]["Description"];
    return desc;
  } else {
      QString desc = this->dvarsDescriptions[this->varname.c_str()]["Description"];
      return desc;
  }
}

void DataOutputItem::setData(bool set) {
  if (this->__parent) {
    auto sel = this->proto->measureMgr().selection();
    auto varSel = sel[this->varname];
    if (set) {
      varSel.insert(this->measname);
    } else {
      varSel.erase(this->measname);
    }
    sel[this->varname] = varSel;
    this->proto->measureMgr().selection(sel);
  } else {
    auto sel = this->proto->cell()->getVariableSelection();
    if (set) {
      sel.insert(this->varname);
    } else {
      sel.erase(this->varname);
    }
    this->proto->cell()->setVariableSelection(sel);
  }
}

bool DataOutputItem::getData() const {
  if (this->__parent) {
    auto sel = this->proto->measureMgr().selection();
    return sel[this->varname].count(this->measname) > 0;
  } else {
    auto sel = this->proto->cell()->getVariableSelection();
    return sel.count(this->varname) > 0;
  }
}

QString DataOutputItem::getType() const {
  QMap<QString, QRegExp> groups = {
      {"Gates", QRegExp("^Gate.", Qt::CaseInsensitive)},
      {"Currents", QRegExp("^i")},
      {"Calcium Concentrations", QRegExp("^ca")}};
  for (auto group = groups.begin(); group != groups.end(); ++group) {
    if (group->indexIn(this->varname.c_str()) != -1) {
      return group.key();
    }
  }
  return "Other";
}

bool DataOutputItem::isMeasItem() const { return this->measname != ""; }
