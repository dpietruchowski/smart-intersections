#include "CarItem.h"

namespace {

const QPolygonF carShape = QPolygonF({
    QPointF{15, -23}, QPointF{15, 18}, QPointF{12, 23},
    QPointF{-12, 23}, QPointF{-15, 18}, QPointF{-15, -23}
});

}

CarItem::CarItem(QGraphicsItem* parent): QGraphicsPolygonItem(parent)
{
    setPolygon(carShape);
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

void CarItem::onStep()
{
    distance_ += velocity_;
}

qreal CarItem::getVelocity() const
{
    return velocity_;
}
