/*
 * the main window for LongQt contains all the pretinant widgets and maintains
 * the side pannel to switch between them
 */
#ifndef HEART_CELL_SIM_H
#define HEART_CELL_SIM_H

#include <QComboBox>
#include <QDir>
#include <QFuture>
#include <QFutureWatcher>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QList>
#include <QListWidget>
#include <QMainWindow>
#include <QProgressBar>
#include <QPushButton>
#include <QSpinBox>
#include <QStackedWidget>

#include "protocol.h"

namespace LQ = LongQt;

namespace Ui {
class LongQtMainWindow;
}

class LongQtMainWindow : public QMainWindow {
  Q_OBJECT

 public:
  LongQtMainWindow(QString simvarFile = "", QWidget* parent = 0);
  ~LongQtMainWindow();

 private:
  std::shared_ptr<LQ::Protocol> proto;
  Ui::LongQtMainWindow* ui;

  void insertItem(int pos, QWidget* widget, QString name, QString toolTip = "");
  void appendItem(QWidget* widget, QString name, QString toolTip = "");
  void removeItem(int pos);
  void setItemHidden(int pos, bool hidden);

  void nextItem();
  void list_click_aciton(int next_row);
  void finished();
  void changeProto(std::shared_ptr<LQ::Protocol> proto);

 signals:
  void cellChanged(std::shared_ptr<LQ::Cell>);
  void protocolChanged(std::shared_ptr<LQ::Protocol>);
};

#endif  // HEART_CELL_SIM_H
