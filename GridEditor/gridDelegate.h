#ifndef GRID_DELEGATE_H
#define GRID_DELEGATE_H
#include <QStyledItemDelegate>

class GridDelegate : public QStyledItemDelegate
{
	Q_OBJECT
	public:
		GridDelegate(QWidget *parent = 0);

		void paint(QPainter *painter, const QStyleOptionViewItem &option,
				const QModelIndex &index) const;
		QSize sizeHint(const QStyleOptionViewItem &option,
				const QModelIndex &index) const;
		QWidget* createEditor(QWidget *parent, const QStyleOptionViewItem &option,
				const QModelIndex &index) const;
/*		void setEditorData(QWidget *editor, const QModelIndex &index) const;
		void setModelData(QWidget *editor, QAbstractItemModel *model,
				const QModelIndex &index) const;*/
	private:
		QSize size;
		QMap<QString,QPalette> colors; 
};
#endif
