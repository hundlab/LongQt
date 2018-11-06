#ifndef VOLTAGECLAMPDELEGATE_H
#define VOLTAGECLAMPDELEGATE_H
#include <QStyledItemDelegate>

class VoltageClampDelegate : public QStyledItemDelegate
{
Q_OBJECT
public:
    VoltageClampDelegate(QWidget *parent = 0);
    virtual QWidget* createEditor(QWidget *parent, const QStyleOptionViewItem &option,
            const QModelIndex &index) const override;
    virtual void setEditorData(QWidget *editor, const QModelIndex &index) const override;
    virtual void setModelData(QWidget *editor, QAbstractItemModel *model,
            const QModelIndex &index) const override;
    virtual void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option,
            const QModelIndex &index) const override;
};

#endif // VOLTAGECLAMPDELEGATE_H
