#include "CarItem.h"

namespace {

const QPolygonF carShape = QPolygonF({
    QPointF{15, -23}, QPointF{15, 18}, QPointF{12, 23},
    QPointF{-12, 23}, QPointF{-15, 18}, QPointF{-15, -23}
});

}

CarItem::CarItem(int id, QGraphicsItem* parent): QGraphicsPolygonItem(parent), BaseItem(id)
{
    setPolygon(carShape);
    auto* text = new QGraphicsTextItem(QString::number(id), this);
}

CarItem::CarItem(QGraphicsItem* parent): CarItem(-1, parent)
{

}

void CarItem::setVelocity(qreal velocity)
{
    velocity_ = velocity;
}

void CarItem::setDistance(qreal distance)
{
    distance_ = distance;
}

qreal CarItem::getDistance() const
{
    return distance_;
}

bool CarItem::load(QXmlStreamReader& xmlStream)
{
    if (!xmlStream.isStartElement() ||
            xmlStream.name() != "car")
        return false;

    int id = xmlStream.attributes().value("id").toInt();
    qreal v = xmlStream.attributes().value("v").toDouble();
    qreal d = xmlStream.attributes().value("d").toDouble();
    setId(id);
    setVelocity(v);
    setDistance(d);
    xmlStream.skipCurrentElement();
    return true;
}

void CarItem::save(QXmlStreamWriter& xmlStream) const
{
    xmlStream.writeStartElement("car");
    xmlStream.writeAttribute("id", QString::number(getId()));
    xmlStream.writeAttribute("v", QString::number(getVelocity()));
    xmlStream.writeAttribute("d", QString::number(getDistance()));
    xmlStream.writeEndElement();
}

void CarItem::onStep()
{
    distance_ += velocity_;
}

qreal CarItem::getVelocity() const
{
    return velocity_;
}
