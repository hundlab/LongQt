#include "gridDelegate.h"
#include "cellutils.h"
#include "guiUtils.h"
#include "inexcitablecell.h"

#include <QPainter>
#include <QPen>
#include <QSizePolicy>
using namespace std;
using namespace LongQt;

GridDelegate::GridDelegate(QWidget *parent)
    : QStyledItemDelegate(parent), size(80, 30) {
  map<string, CellUtils::CellInitializer> cellMap = CellUtils::cellMap;
  cellMap["Inexcitable Cell"] = []() { return make_shared<InexcitableCell>(); };
  int i = 0;
  for (auto &pair : cellMap) {
    QPalette palette;
    if (pair.first == "Inexcitable Cell") {
      auto white = QColor(Qt::white);
      palette.setColor(QPalette::Base, white);
      white.setAlpha(120);
      palette.setColor(QPalette::Highlight, white);
      palette.setColor(QPalette::HighlightedText, white);
    } else {
      palette.setColor(QPalette::Base, GuiUtils::getLightColor(i));
      palette.setColor(QPalette::Highlight, GuiUtils::getLightColor(i, 120));
      palette.setColor(QPalette::HighlightedText,
                       GuiUtils::getLightColor(i, 120));
    }

    this->colors.insert(pair.first.c_str(), palette);
    i++;
  }
}

void GridDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option,
                         const QModelIndex &index) const {
  const QPalette &type = this->colors.value(index.data().toString());
  painter->save();
  QPen pen(Qt::black);
  pen.setWidth(3);
  painter->setPen(pen);
  if (option.state & QStyle::State_Selected) {
    painter->fillRect(option.rect, type.highlight());
  } else {
    painter->fillRect(option.rect, type.base());
  }
  // meas status
  if (index.child(0, 1).data().toBool()) {
    QRect rectangle(option.rect.left() + 2, option.rect.top() + 2,
                    option.rect.width() - 2, option.rect.height() - 2);
    if (!rectangle.isValid()) {
      return;
    }
    int startAngle = 0;
    int spanAngle = 360 * 16;
    painter->drawArc(rectangle, startAngle, spanAngle);
  }
  // stim status
  if (index.child(0, 0).data().toBool()) {
    QLine line1(option.rect.left(), option.rect.top(), option.rect.right(),
                option.rect.bottom());
    QLine line2(option.rect.left(), option.rect.bottom(), option.rect.right(),
                option.rect.top());
    painter->drawLine(line1);
    painter->drawLine(line2);
  }
  painter->drawRect(option.rect);
  painter->restore();
}

QSize GridDelegate::sizeHint(const QStyleOptionViewItem &,
                             const QModelIndex &) const {
  return this->size;
}
