#ifndef PROTOCHOOSER_H
#define PROTOCHOOSER_H

#include <QButtonGroup>
#include <QGroupBox>
#include <QWidget>
#include <memory>
#include "protocol.h"

namespace LQ = LongQt;

class ProtoChooser : public QObject {
  Q_OBJECT

 public:
  explicit ProtoChooser(QGroupBox* parent = 0);
  ~ProtoChooser();

  void update_ui();
  void update_model(int index);
  void changeProto(std::shared_ptr<LQ::Protocol> proto);

  std::shared_ptr<LQ::Protocol> proto;

 signals:
  void protocolChanged(std::shared_ptr<LQ::Protocol>);
  void cellChanged(std::shared_ptr<LQ::Cell>);

 private:
  QList<std::string> protoNums = {"Current Clamp Protocol",
                                  "Voltage Clamp Protocol", "Grid Protocol"};
  QButtonGroup* group;
};

#endif  // PROTOCHOOSER_H
