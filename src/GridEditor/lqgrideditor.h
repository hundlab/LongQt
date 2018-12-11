/*
 * The main window for the LQGridEditor.pro
 * contains the other widgets which alow for configuring the grid
 */
#ifndef LQGRIDEDITOR_H
#define LQGRIDEDITOR_H

#include <QMainWindow>
#include <QPointer>
#include <QTableView>
#include "conductivityeditor.h"
#include "ionchannelconfig.h"
namespace LQ = LongQt;

namespace Ui {
class LQGridEditor;
}

class LQGridEditor : public QMainWindow {
  Q_OBJECT

 public:
  explicit LQGridEditor(QWidget *parent = 0);
  ~LQGridEditor();

 private:
  Ui::LQGridEditor *ui;
  QPointer<QTableView> gridView;
  QPointer<ConductivityEditor> condEdit;
  QPointer<IonChannelConfig> ionConfig;
  std::shared_ptr<LQ::GridProtocol> proto = 0;
  QString saveFile = "";
 private slots:
  void on_actionNew_triggered();
  void on_actionOpen_triggered();
  void on_actionSave_triggered();
  void on_actionSave_As_triggered();
  void on_actionSet_Conductivities_triggered();
  void on_actionConfigure_Ion_Channels_triggered();
  void on_actionToggle_Second_Stim_triggered();
  void on_actionSet_Sim_Parameters_triggered();
};

#endif  // LQGRIDEDITOR_H
