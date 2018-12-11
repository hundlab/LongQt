#ifndef SIMVAR_H
#define SIMVAR_H

#include <protocol.h>
#include <QWidget>
namespace LQ = LongQt;

class Simvar : public QWidget {
  Q_OBJECT
 public:
  explicit Simvar(std::shared_ptr<LQ::Protocol> proto, std::string name,
                  QWidget *parent = 0);
  ~Simvar();

  virtual void update_ui() = 0;

 protected:
  std::shared_ptr<LQ::Protocol> proto;
  std::string name;
 public slots:
  virtual void changeProto(std::shared_ptr<LQ::Protocol> proto);
  virtual void changeCell(std::shared_ptr<LQ::Cell>);
 signals:
  void cellChanged(std::shared_ptr<LQ::Cell>);
  void updated();
};

#endif  // SIMVAR_H
