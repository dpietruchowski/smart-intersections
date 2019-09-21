#include "CollisionAreaItem.h"

#include <QBrush>

CollisionAreaItem::CollisionAreaItem(int id, QGraphicsItem* parent): QGraphicsRectItem(parent), BaseItem(id)
{
}

CollisionAreaItem::CollisionAreaItem(QGraphicsItem* parent): CollisionAreaItem(-1, parent)
{

}

bool CollisionAreaItem::containsGlobal(const QPointF point) const
{
    return contains(point - pos());
}

void CollisionAreaItem::setOccupied(bool occupied)
{
    occupied_ = occupied;
    if (isOccupied()) {
        setBrush(QColor(91, 129, 213, 155));
    } else {
        setBrush(QBrush());
    }
    update();
}

void CollisionAreaItem::onReset()
{
    setOccupied(false);
}

const char* CollisionAreaItem::getItemName()
{
    return "collision-area";
}

bool CollisionAreaItem::loadItem(QXmlStreamReader& xmlStream)
{
    qreal x = xmlStream.attributes().value("x").toDouble();
    qreal y = xmlStream.attributes().value("y").toDouble();
    setPos(x, y);

    qreal w = xmlStream.attributes().value("w").toDouble();
    qreal h = xmlStream.attributes().value("h").toDouble();
    setRect(0, 0, w, h);

    xmlStream.skipCurrentElement();

    return true;
}

void CollisionAreaItem::saveItem(QXmlStreamWriter& xmlStream) const
{
    QPointF point = pos();
    xmlStream.writeAttribute("x", QString::number(point.x()));
    xmlStream.writeAttribute("y", QString::number(point.y()));

    QRectF r = rect();
    xmlStream.writeAttribute("w", QString::number(r.width()));
    xmlStream.writeAttribute("h", QString::number(r.height()));
}
