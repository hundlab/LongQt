/*
 * the main window for LongQt contains all the pretinant widgets and maintains
 * the side pannel to switch between them
 */
#ifndef HEART_CELL_SIM_H
#define HEART_CELL_SIM_H

#include <QPushButton>
#include <QLabel>
#include <QSpinBox>
#include <QComboBox>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QStackedWidget>
#include <QListWidget>
#include <QList>
#include <QProgressBar>
#include <QFuture>
#include <QFutureWatcher>
#include <QLineEdit>
#include <QDir>

#include "protocol.h"
#include "settingsIO.h"


class Simulation : public QWidget {
    Q_OBJECT

  public:
    Simulation(QString simvarFile = "", QString dvarFile = "", QString measFile = "", QString pvarFile = "", QWidget* parent = 0);
    ~Simulation();

  private:
    QWidget* parent;
    Protocol* proto;
    QString date_time;
    QList<QWidget*> menu_list;
//utility functions & classes
	SettingsIO settingsMgmt;
    void leave_current(int current);
//buttons
    QPushButton* next_button;
    QPushButton* cancel_button;
    QPushButton* about_button;
//organizational widgets
    QListWidget* menu_options;
    QStackedWidget* menu;
//layouts
    QGridLayout* main_layout;

  private slots:
    void next_button_aciton();
    void list_click_aciton (int next_row);
    void canceled();
    void finished();
    void running();
    void changeProto(Protocol* proto);
  signals:
    void cell_type_changed();
    void working_dir_changed(QDir& dir);
};

#endif // HEART_CELL_SIM_H
