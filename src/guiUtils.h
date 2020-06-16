/*
 * class GuiTuils is for generic methods or data used by any part of the gui
 *
 * color schemes are borrowed from Paul Tol
 * https://personal.sron.nl/~pault
 */

#ifndef GUIUTILS_H
#define GUIUTILS_H
#include <QColor>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QTextStream>

#include "cellutils_core.h"

namespace GuiUtils {
// version number for LongQt
static const char* version = LongQt::CellUtils::version;

// used to read in hoverTexts from thier files
inline QMap<QString, QString> readMap(QString fileName,
                                      QString cellType = "Default") {
  QMap<QString, QString> map;
  QFile* file = new QFile(fileName);
  file->open(QIODevice::ReadOnly | QIODevice::Text);
  if (!file->isOpen()) return map;
  QJsonDocument doc = QJsonDocument::fromJson(file->readAll());
  if (doc.isNull()) return map;
  file->close();
  delete file;

  QJsonObject jmap = doc.object();
  auto vmap = jmap["Default"].toObject().toVariantMap();
  for (auto it = vmap.begin(); it != vmap.end(); ++it) {
    map[it.key()] = it.value().toString();
  }
  if (!jmap.contains(cellType) || cellType == "Default") return map;
  vmap = jmap[cellType].toObject().toVariantMap();
  for (auto it = vmap.begin(); it != vmap.end(); ++it) {
    map[it.key()] = it.value().toString();
  }

  return map;
}

inline QMap<QString, QMap<QString, QString>> readDesc(QString fileName) {
  QMap<QString, QMap<QString, QString>> map;
  QFile* file = new QFile(fileName);
  file->open(QIODevice::ReadOnly | QIODevice::Text);
  if (!file->isOpen()) return map;
  QJsonDocument doc = QJsonDocument::fromJson(file->readAll());
  if (doc.isNull()) return map;
  file->close();
  delete file;

  QJsonObject jmap = doc.object();
  for (auto it = jmap.begin(); it != jmap.end(); ++it) {
    QMap<QString, QString> entryMap;
    auto value = it.value().toObject();
    for (auto jt = value.begin(); jt != value.end(); ++jt) {
      entryMap[jt.key()] = jt.value().toString();
    }
    map[it.key()] = entryMap;
  }
  return map;
}

// also for hoverTexts (add units &etc)
inline QMap<QString, QString> concatMaps(QMap<QString, QString> m1,
                                         QString divider,
                                         QMap<QString, QString> m2,
                                         QString blankVal = "") {
  QMap<QString, QString> returnMap;
  QString blankEnd = "";
  if (blankVal != "") {
    blankEnd = divider + blankVal;
  }
  for (auto it1 = m1.begin(); it1 != m1.end(); it1++) {
    auto m2Pair = m2.find(it1.key());
    if (m2Pair != m2.end() && m2Pair.value() != "") {
      returnMap[it1.key()] = it1.value() + divider + m2Pair.value();
    } else {
      returnMap[it1.key()] = it1.value() + blankEnd;
    }
  }
  return returnMap;
}

inline QColor getLightColor(int num, int saturation = -1) {
  QList<QString> colors = {"#77AADD", "#EE8866", "#EEDD88", "#FFAABB",
                           "#99DDFF", "#44BB99", "#BBCC33", "#AAAA00",
                           "#DDDDDD", "#000000"};

  auto color = QColor(colors[int(num % colors.length())]);
  if (saturation >= 0 && saturation <= 255) {
    color.setAlpha(saturation);
    return color;
  }
  return color;
}

// used by gridEditor and Grapher to get colors that wont repeat
inline QColor genMutedColor(int num, int saturation = -1) {
  // return QColor::fromHsv((num * 4 * 17) % 360, saturation, 200);
  QList<QString> colors = {"#4477aa", "#66ccee", "#228833", "#ccbb44",
                           "#ee6677", "#aa3377", "#bbbbbb"};
  auto color = QColor(colors[int(num % colors.length())]);
  if (saturation >= 0 && saturation <= 255) {
    color.setAlpha(saturation);
    return color;
  }
  return color;
}

inline QString getName(int trial, std::vector<int> cellInfo,
                       QString prefix = "") {
  if (cellInfo.size() > 1) {
    return prefix + "Cell " + QString::number(cellInfo[0]) + " " +
           QString::number(cellInfo[1]);
  } else {
    return prefix + "Trial " + QString::number(trial);
  }
}
}  // namespace GuiUtils
#endif
