#ifndef ADDGRIDCELLPVAR_H
#define ADDGRIDCELLPVAR_H

#include <QDialog>
#include <QMap>
#include <QPointer>
#include <QString>
#include <QTableView>
#include "gridProtocol.h"
namespace LQ = LongQt;

namespace Ui {
class AddGridCellPvar;
}

class AddGridCellPvar : public QDialog {
  Q_OBJECT

 public:
  explicit AddGridCellPvar(QTableView *view,
                           std::shared_ptr<LQ::GridProtocol> proto,
                           QWidget *parent = 0);
  ~AddGridCellPvar();

 private:
  void updateIonChannelType();
  std::set<std::pair<int, int>> getInitial();

  std::shared_ptr<LQ::GridProtocol> proto;
  QMap<QString, QString> pvarsDescriptions;
  Ui::AddGridCellPvar *ui;
  QPointer<QTableView> view = 0;

 public slots:
  void changeProto(std::shared_ptr<LQ::GridProtocol> proto);
  void changeCell(std::shared_ptr<LQ::Cell> cell);

 signals:
  void pvarsChanged();

 private slots:
  void on_randomize_stateChanged(int state);
  void on_multiple_stateChanged(int state);
  void on_normalDist_toggled(bool checked);
  void on_addButton_clicked();
};

#endif  // ADDGRIDCELLPVAR_H
