#ifndef SPECIALMENU_H
#define SPECIALMENU_H

#include <QMap>
#include <QString>
#include <QWidget>
#include <memory>
#include <string>
#include "protocol.h"

namespace LQ = LongQt;

class SpecialMenu : public QWidget {
  Q_OBJECT
 public:
  explicit SpecialMenu(QWidget *parent = nullptr);

  void changeProto(std::shared_ptr<LQ::Protocol> proto);
  void changeCell(std::shared_ptr<LQ::Cell> cell);

 private:
  typedef std::function<QWidget *(std::shared_ptr<LQ::Protocol>)> WidgetMaker;
  QMap<std::string, WidgetMaker> specialMenus;
  QMap<std::string, QString> names;

 signals:
  void cellChanged(std::shared_ptr<LQ::Cell>);
  void protocolChanged(std::shared_ptr<LQ::Protocol>);
  void nameChanged(QString name);
  void noWidget();

  void cellChangedInternal(std::shared_ptr<LQ::Cell>);
  void protocolChangedInternal(std::shared_ptr<LQ::Protocol>);
};

#endif  // SPECIALMENU_H
