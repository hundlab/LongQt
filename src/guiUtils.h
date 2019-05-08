/*
 * class GuiTuils is for generic methods or data used by any part of the gui
 *
 */

#ifndef GUIUTILS_H
#define GUIUTILS_H
#include <QColor>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QTextStream>

namespace GuiUtils {
// version number for LongQt
static const char* version = "v0.4";

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
// used by gridEditor and Grapher to get colors that wont repeat
inline QColor genColor(int num, int saturation = 200) {
  // return QColor::fromHsv((num * 4 * 17) % 360, saturation, 200);
  QList<QString> colors = {"#4477aa", "#66ccee", "#228833", "#ccbb44",
                           "#ee6677", "#aa3377", "#bbbbbb"};
  return QColor(colors[int(num % colors.length())]);
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
