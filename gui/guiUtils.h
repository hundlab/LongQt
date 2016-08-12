#ifndef GUIUTILS_H
#define GUIUTILS_H

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
};
#endif
