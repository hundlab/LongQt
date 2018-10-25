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
namespace LQ = LongQt;

namespace Ui {
    class MvarMenu;
}

class MvarMenu: public QWidget {
Q_OBJECT
  public:
    explicit MvarMenu(std::shared_ptr<LQ::Protocol> proto, QWidget* parent = 0);
   ~MvarMenu();

  private:
    void setupMenu();
    QString getType(QString name);

    std::shared_ptr<LQ::Protocol> proto;
    QMap<QString,QString> dvarsDescriptions;
    QMap<QString,QString> measDescriptions;
    QVector<std::string> cellVars;
    QVector<std::string> measVars;
    Ui::MvarMenu *ui;
  private slots:
    void addMeas(QTreeWidgetItem * item, int column);
    void childMeas(QTreeWidgetItem * item, int column);
    void parentMeas(QTreeWidgetItem * item, int column);
    void setParentCheckedState(QTreeWidgetItem* item, int column);
    void setChildrenCheckedStates(QTreeWidgetItem* item, int column, Qt::CheckState state);
  public slots:
    void changeProto(std::shared_ptr<LQ::Protocol> proto);
    void changeCell(std::shared_ptr<LQ::Cell> cell);
    void reset();
};

#endif
