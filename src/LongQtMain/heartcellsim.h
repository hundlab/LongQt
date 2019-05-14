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
#include <QProgressBar>
#include <QPushButton>
#include <QSpinBox>
#include <QStackedWidget>

#include "protocol.h"
namespace LQ = LongQt;

class Simulation : public QWidget {
  Q_OBJECT

 public:
  Simulation(QString simvarFile = "", QWidget* parent = 0);
  ~Simulation();

 private:
  QWidget* parent;
  std::shared_ptr<LQ::Protocol> proto;
  QString date_time;
  QList<QWidget*> menu_list;
  // buttons
  QPushButton* next_button;
  QPushButton* cancel_button;
  QPushButton* about_button;
  // organizational widgets
  QListWidget* menu_options;
  QStackedWidget* menu;
  // layouts
  QGridLayout* main_layout;

 private slots:
  void next_button_aciton();
  void list_click_aciton(int next_row);
  void canceled();
  void finished();
  void running();
  void changeProto(std::shared_ptr<LQ::Protocol> proto);
 signals:
  void cellChanged(std::shared_ptr<LQ::Cell>);
  void working_dir_changed(QDir& dir);
};

#endif  // HEART_CELL_SIM_H
