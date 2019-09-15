#include "CarItem.h"

#include "IntersectionScene.h"
#include "PathItem.h"

namespace {

const QPolygonF carShape = QPolygonF({
    QPointF{15, -23}, QPointF{15, 18}, QPointF{12, 23},
    QPointF{-12, 23}, QPointF{-15, 18}, QPointF{-15, -23}
});

}

CarItem::CarItem(int id, QGraphicsItem* parent): QGraphicsPolygonItem(parent), BaseItem(id)
{
    setPolygon(carShape);
    text_ = new QGraphicsSimpleTextItem(QString::number(id), this);
}

CarItem::CarItem(QGraphicsItem* parent): CarItem(-1, parent)
{

}

void CarItem::reset()
{
    if (route_)
        routeIter_ = route_->begin();

    setDistance(getDefaultDistance());
}

void CarItem::setVelocity(qreal velocity)
{
    velocity_ = velocity;
}

void CarItem::setDistance(qreal distance)
{
    distance_ = distance;
}

void CarItem::setDefaultDistance(qreal distance)
{
    defaultDistance_ = distance;
}

qreal CarItem::getDistance() const
{
    return distance_;
}

qreal CarItem::getDefaultDistance() const
{
    return defaultDistance_;
}

void CarItem::setRoute(Route* route)
{
    route_ = route;
}

PathItem* CarItem::getNextPath()
{
    if (routeIter_ == route_->end())
        return nullptr;
    return *routeIter_++;
}

void CarItem::moveToNextPath(qreal distance)
{
    PathItem* nextPath = getNextPath();
    if (!nextPath)
        return;

    setDistance(distance);
    nextPath->addCar(this);
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

    if (xmlStream.readNextStartElement() && xmlStream.name() == "route-id") {
        QString strPathId = xmlStream.readElementText();
        IntersectionScene* intersection = dynamic_cast<IntersectionScene*>(scene());
        if (intersection) {
            route_ = intersection->getRoute(strPathId.toInt());
        }
    }
    xmlStream.skipCurrentElement();
    return true;
}

void CarItem::save(QXmlStreamWriter& xmlStream) const
{
    xmlStream.writeStartElement("car");
    xmlStream.writeAttribute("id", QString::number(getId()));
    xmlStream.writeAttribute("v", QString::number(getVelocity()));
    xmlStream.writeAttribute("d", QString::number(getDistance()));

    if (route_)
        xmlStream.writeTextElement("route-id",
                                   QString::number(route_->getId()));

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
