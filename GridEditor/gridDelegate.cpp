#include "gridDelegate.h"
#include "cellUtils.h"
#include "guiUtils.h"

#include <QPen>
#include <QPainter>
#include <QSizePolicy>
#include <QDebug>

GridDelegate::GridDelegate(QWidget *parent) : QStyledItemDelegate(parent), size(30,30) {
	map<string, CellInitializer> cellMap = cellUtils().cellMap;
	cellMap["Inexcitable Cell"] = [] () {return new Cell;};	
	int i = 0;
	for(auto& pair : cellMap) {
		QPalette* palette = new QPalette();
		palette->setColor(QPalette::Base, GuiUtils().genColor(i));
		palette->setColor(QPalette::Highlight, GuiUtils().genColor(i,80));
		palette->setColor(QPalette::HighlightedText, GuiUtils().genColor(i,80));

		this->colors.insert(pair.first.c_str(), *palette);
		i++;
	}
}

QWidget* GridDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option,const QModelIndex &index) const {
	return 0;
}

void GridDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const {
	const QPalette& type = this->colors.value(index.data().toString());
	painter->save();
	QPen pen(Qt::black);
	pen.setWidth(3);
	painter->setPen(pen);
	if(option.state & QStyle::State_Selected) {
		painter->fillRect(option.rect, type.highlight());
	} else {
		painter->fillRect(option.rect, type.base());
	}
	//meas status
	if(index.child(0,1).data().toBool()) {
		QRect rectangle(option.rect.left()+2, option.rect.top()+2, option.rect.width()-2, option.rect.height()-2);
		if(!rectangle.isValid()) {
			return;
		}
		int startAngle = 0;
		int spanAngle = 360 * 16;
		painter->drawArc(rectangle, startAngle, spanAngle);
	}
	//stim status
	if(index.child(0,0).data().toBool()) {
		QLine line1(option.rect.left(),option.rect.top(),option.rect.right(),option.rect.bottom());
		QLine line2(option.rect.left(), option.rect.bottom(),option.rect.right(),option.rect.top());
		painter->drawLine(line1);
		painter->drawLine(line2);
	}
	painter->drawRect(option.rect);
	painter->restore();
}

QSize GridDelegate::sizeHint(const QStyleOptionViewItem &option,const QModelIndex &index) const {
	return this->size;	
}

