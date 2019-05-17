#ifndef SIMVAR_H
#define SIMVAR_H

#include <protocol.h>
#include <QLabel>
#include <QMap>
#include <QObject>
#include <QWidget>
namespace LQ = LongQt;

class Simvar : public QObject {
  Q_OBJECT
 public:
  explicit Simvar(std::shared_ptr<LQ::Protocol> proto, std::string name,
                  QWidget* parent);
  virtual ~Simvar();

  virtual void setupLabel(QLabel* label);
  virtual void update_ui() = 0;
  virtual void changeProto(std::shared_ptr<LQ::Protocol> proto);
  virtual void changeCell(std::shared_ptr<LQ::Cell>);

  virtual QString getPrettyName();
  virtual QString getToolTip();

  std::shared_ptr<LQ::Protocol> proto;
  std::string name;

  static QMap<QString, QMap<QString, QString>> descriptions;

 signals:
  void updated();
};

#endif  // SIMVAR_H
