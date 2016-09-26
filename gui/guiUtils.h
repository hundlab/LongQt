#ifndef GUIUTILS_H
#define GUIUTILS_H
#include <QTextStream>

class GuiUtils {
  public:
    QMap<QString, QString> readMap(QString fileName) {
        QFile* file = new QFile(fileName);
        QMap<QString, QString> map;
        file->open(QIODevice::ReadOnly|QIODevice::Text);
        if(!file->isOpen()) {
            return map;
        }
        QTextStream fileStream(file);
        while(!fileStream.atEnd()) {
            QString key = fileStream.readLine();
            QString value = fileStream.readLine();
            map.insert(key,value);
        }
        return map;
    }
    QMap<QString, QString> concatMaps(QMap<QString, QString> m1, QString divider, QMap<QString, QString> m2, QString blankVal = "") {
        QMap<QString, QString> returnMap;
        QString blankEnd = "";
        if(blankVal != "") {
            blankEnd = divider + blankVal;
        }
        for(auto it1 = m1.begin(); it1 != m1.end(); it1++) {
            auto m2Pair = m2.find(it1.key());
            if(m2Pair != m2.end() && m2Pair.value() != "") {
                returnMap[it1.key()] = it1.value() + divider + m2Pair.value();
            } else {
                returnMap[it1.key()] = it1.value() + blankEnd;
            }
         }
         return returnMap;
    }
    QColor genColor(int num, int saturation = 200) {
        return QColor::fromHsv((num*4*17)%360,saturation,200);
    }
};
#endif
