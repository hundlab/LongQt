#ifndef CHECKANDWARN_H
#define CHECKANDWARN_H

#include <QDialog>
#include <QMap>
#include <QStringList>
#include <memory>
#include "protocol.h"

namespace Ui {
class CheckAndWarn;
}
class CheckAndWarn : public QDialog {
  Q_OBJECT
 public:
  explicit CheckAndWarn(std::shared_ptr<LongQt::Protocol> proto,
                        QWidget* parent = nullptr);
  ~CheckAndWarn() { delete ui; }

  bool hasWarnings();

 private:
  bool runChecks(std::shared_ptr<LongQt::Protocol> proto);

  bool checkWritetimeTooLarge(std::shared_ptr<LongQt::Protocol> proto);
  bool checkMeasTimeTooLarge(std::shared_ptr<LongQt::Protocol> proto);

  Ui::CheckAndWarn* ui;

  QStringList warningsList;
  QMap<QString, QMap<QString, QString>> simvDesc;
  QMap<QString, QMap<QString, QString>> measDesc;
  QMap<QString, QMap<QString, QString>> cellVarsDesc;
  QMap<QString, QMap<QString, QString>> cellParsDesc;
};

#endif  // CHECKANDWARN_H
