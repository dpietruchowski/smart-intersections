#include "CarItem.h"

#include "IntersectionScene.h"
#include "PathItem.h"

#include <QDebug>

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
}

CarItem::CarItem(QGraphicsItem* parent): CarItem(-1, parent)
{

}

void CarItem::limitCarVelocity(qreal velocity)
{
    if (velocity < maxVelocity_)
        maxVelocity_ = velocity;
}

void CarItem::setVelocity(qreal velocity)
{
    desiredVelocity_ = velocity;
}

void CarItem::setMaxVelocity(qreal velocity)
{
    maxVelocity_ = velocity;
}

void CarItem::setDesiredVelocity(qreal velocity)
{
    desiredVelocity_ = velocity;
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

qreal CarItem::getPrevDistance() const
{
    return prevDistance_;
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

void CarItem::moveToNextPath(qreal distance)
{
    PathItem* nextPath = getNextPath();
    if (!nextPath)
        return;

    setDistance(distance);
    nextPath->addCar(this);
}

void CarItem::onReset()
{
    if (route_)
        routeIter_ = route_->begin();

    setDistance(getDefaultDistance());
}

void CarItem::onStep()
{
    if (maxVelocity_ < desiredVelocity_)
        velocity_ = maxVelocity_;
    else
        velocity_ = desiredVelocity_;

    if (velocity_ < 0)
        qDebug() << velocity_;
    prevDistance_ = distance_;
    distance_ += velocity_;
}

qreal CarItem::getVelocity() const
{
    return velocity_;
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
    setVelocity(v);
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
    xmlStream.writeAttribute("v", QString::number(getVelocity()));
    xmlStream.writeAttribute("d", QString::number(getDefaultDistance()));

    if (route_)
        xmlStream.writeTextElement("route-id",
                                   QString::number(route_->getId()));
}
