#include "CarPathItem.h"

#include "CarItem.h"

CarPathItem::CarPathItem(QGraphicsItem* parent): QGraphicsPathItem(parent)
{

}

CarItem* CarPathItem::addCar(qreal velocity, qreal distance)
{
    CarItem* car = new CarItem(this);
    car->setVelocity(velocity);
    car->setDistance(distance);
    updateCar(car);

    cars_.push_back(car);
    return car;
}

void CarPathItem::onStep()
{
    for (auto* car : cars_) {
        updateCar(car);
        if (percentAtCar(car) >= 100) {
            cars_.remove(car);
            delete car;
        }
    }
}

std::pair<QPointF, qreal> CarPathItem::pointAtCar(const CarItem* car)
{
    QPainterPath _path = path();
    qreal percent = _path.percentAtLength(car->getDistance());
    QPointF pos = _path.pointAtPercent(percent);
    qreal angle = _path.angleAtPercent(percent);
    return std::make_pair(pos, angle);
}

qreal CarPathItem::percentAtCar(const CarItem* car)
{
    return path().percentAtLength(car->getDistance());
}

void CarPathItem::updateCar(CarItem* car)
{
    auto positionAngle = pointAtCar(car);
    car->setPos(positionAngle.first);
    car->setRotation(270-positionAngle.second);
}
