#ifndef ONEITEMLAYOUT_H
#define ONEITEMLAYOUT_H

#include <QWidget>
#include <QScopedPointer>
#include <QLayout>


class OneItemLayout : public QLayout
{
public:
    OneItemLayout();
    OneItemLayout(QWidget* parent);
    virtual ~OneItemLayout();
    void setGeometry(const QRect &r);
    QSize sizeHint() const;
    QSize minimumSize() const;
    bool hasHeightForWidth() const;
    int heightForWidth(int w) const;
    void addItem(QLayoutItem *item);
    QLayoutItem *itemAt(int index) const;
    QLayoutItem *takeAt(int index);
    int count() const;
private:
    QScopedPointer<QLayoutItem> item;
};

#endif // ONEITEMLAYOUT_H
