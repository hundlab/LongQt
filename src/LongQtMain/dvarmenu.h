/*
 * Dvars are the output or tracked variables from a sim. They can be found in
 * cell->vars
 */
#ifndef DVARMENU_H
#define DVARMENU_H

#include <QCheckBox>
#include <QVector>
#include <QWidget>

#include "cell.h"
namespace LQ = LongQt;

class dvarMenu : public QWidget {
  Q_OBJECT
 public:
  dvarMenu(std::shared_ptr<LQ::Cell> cell, QWidget* parent = 0);
  void createMenu();
  ~dvarMenu();

 private:
  std::shared_ptr<LQ::Cell> cell;

  // Buttons & their labels
  QVector<QCheckBox*> dvars;
  // screen functions
  void update_menu();  // make menu match pars

 private slots:
  void update_datamap(
      std::string p,
      int state);  // make a Protocol::pars entry match the screen
 public slots:
  void changeCell(std::shared_ptr<LQ::Cell> cell);
  void reset();
};

#endif
