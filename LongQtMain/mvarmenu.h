/*
 * Measurement variables are the cell->vars which will have basic measurements
 * taken of them durring the simulation
 * This widget alows for them to be added and removed from the mvars list
 */
#ifndef MVARMENU_H
#define MVARMENU_H

#include <QWidget>
#include <QMap>
#include <QVector>
#include <QTreeWidgetItem>

#include "protocol.h"

namespace Ui {
    class MvarMenu;
}

using namespace std;

class MvarMenu: public QWidget {
Q_OBJECT
  public:
    explicit MvarMenu(shared_ptr<Protocol> proto, QWidget* parent = 0);
   ~MvarMenu();

  private:
    void setupMenu();
    QString getType(QString name);

    shared_ptr<Protocol> proto;
    QMap<QString,QString> dvarsDescriptions;
    QMap<QString,QString> measDescriptions;
    QVector<string> cellVars;
    QVector<string> measVars;
    Ui::MvarMenu *ui;
  private slots:
    void addMeas(QTreeWidgetItem * item, int column);
    void childMeas(QTreeWidgetItem * item, int column);
    void parentMeas(QTreeWidgetItem * item, int column);
    void setParentCheckedState(QTreeWidgetItem* item, int column);
    void setChildrenCheckedStates(QTreeWidgetItem* item, int column, Qt::CheckState state);
  public slots:
    void changeProto(shared_ptr<Protocol> proto);
    void changeCell(Cell* cell);
    void reset();
};

#endif
