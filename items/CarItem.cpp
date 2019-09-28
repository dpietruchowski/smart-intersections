#include "CarItem.h"

#include "IntersectionScene.h"
#include "PathItem.h"

#include <QDebug>
#include <QPainter>

namespace {

const QPolygonF carShape = QPolygonF({
    QPointF{12, -23}, QPointF{12, 18}, QPointF{10, 23},
    QPointF{-10, 23}, QPointF{-12, 18}, QPointF{-12, -23}
});

}

CarItem::CarItem(int id, QGraphicsItem* parent): QGraphicsPolygonItem(parent), BaseItem(id)
{
    setPolygon(carShape);
    text_ = new QGraphicsSimpleTextItem(QString::number(id), this);
    text_->setFlag(ItemIgnoresTransformations);
}

CarItem::CarItem(QGraphicsItem* parent): CarItem(-1, parent)
{

}

void CarItem::limitCarVelocity(qreal velocity)
{
    if (velocity < maxVelocity_)
        setMaxVelocity(velocity);
}

void CarItem::setMaxVelocity(qreal velocity)
{
    if (velocity < 0)
        velocity = 0;

    maxVelocity_ = velocity;
}

qreal CarItem::getMaxVelocity() const
{
    return maxVelocity_;
}

void CarItem::setDesiredVelocity(qreal velocity)
{
    if (velocity < 0)
        velocity = 0;
    desiredVelocity_ = velocity;
}

qreal CarItem::getDesiredVelocity() const
{
    return desiredVelocity_;
}

qreal CarItem::getDistance(CarItem::CarPart part) const
{
    return getDistance(distance_, part);
}

qreal CarItem::getRouteDistance(CarItem::CarPart part) const
{
    return getDistance(routeDistance_ + getDistance(), part);
}

qreal CarItem::getNextDistance(CarItem::CarPart part) const
{
    qreal velocity = desiredVelocity_;
    if (maxVelocity_ < desiredVelocity_)
        velocity = maxVelocity_;

    return getDistance(distance_ + velocity, part);
}

qreal CarItem::getLength() const
{
    return getDistance(0, CarPart::Front) - getDistance(0, CarPart::Back);
}

void CarItem::setDistance(qreal distance)
{
    distance_ = distance;
}

void CarItem::setDefaultDistance(qreal distance)
{
    defaultDistance_ = distance;
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
    if (!route_)
        return nullptr;
    if (routeIter_ == route_->end())
        return nullptr;

    return *routeIter_++;
}

void CarItem::moveToRouteDistance(qreal routeDistance)
{
    if (!route_)
        return;

    auto path = route_->getPathAtDistance(routeDistance);
    if (!path.first)
        return;

    setDistance(path.second);
    path.first->addCar(this);
}

void CarItem::moveToNextPath()
{
    routeDistance_ += getDistance();
    moveToRouteDistance(routeDistance_);
}

void CarItem::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
    QGraphicsPolygonItem::paint(painter, option, widget);

    painter->setBrush(QBrush(Qt::red));
    painter->drawEllipse(0, 0, 3, 3);
}

void CarItem::onReset()
{
    routeDistance_ = 0;
    moveToRouteDistance(getDefaultDistance());
    setBrush(QBrush());
}

qreal CarItem::getDistance(qreal distance, CarItem::CarPart part) const
{
    switch (part) {
        case CarPart::Front: return distance + 30;
        case CarPart::Center: return distance;
        case CarPart::Back: return distance - 30;
    }
    return distance;
}

void CarItem::onStep()
{
    distance_ = getNextDistance();
}

const char* CarItem::getItemName()
{
    return "car";
}

bool CarItem::loadItem(QXmlStreamReader& xmlStream)
{
    int id = xmlStream.attributes().value("id").toInt();
    qreal v = xmlStream.attributes().value("v").toDouble();
    qreal d = xmlStream.attributes().value("d").toDouble();
    setId(id);
    setDesiredVelocity(v);
    setDefaultDistance(d);

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

void CarItem::saveItem(QXmlStreamWriter& xmlStream) const
{
    xmlStream.writeAttribute("v", QString::number(getDesiredVelocity()));
    xmlStream.writeAttribute("d", QString::number(getDefaultDistance()));

    if (route_)
        xmlStream.writeTextElement("route-id",
                                   QString::number(route_->getId()));
}
