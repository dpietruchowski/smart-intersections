#include "CarAgent.h"

#include "CarItem.h"

CarAgent::CarAgent(CarItem* car): car_(car)
{
    car_->setAgent(this);
    defaultVelocity_ = car->getDesiredVelocity();
}

CarAgent::~CarAgent()
{
    if (car_)
        car_->setAgent(nullptr);
}

void CarAgent::setCar(CarItem* car)
{
    car_ = car;
}

qreal CarAgent::countDiffDistance(CarAgent::TimespanAtCollisionArea& currentTimespan)
{
    // 0.1 -- look at PathItem::onPreStep() it is required to limit vmax (just before area)
    return currentTimespan.path.getInDistance() - car_->getRouteDistance(CarItem::Front) + 0.1;
}

qreal CarAgent::countAreaLength(CarAgent::TimespanAtCollisionArea& currentTimespan)
{
    return currentTimespan.path.getLength() + car_->getLength() + 1;
}

void CarAgent::findCollisionPaths()
{
    if (!car_)
        return;
    if (!car_->route_)
        return;

    auto collisionPaths = car_->route_->getCollisionPaths();
    for (auto& collisionPath: collisionPaths) {
        timespansRegister.push_back({0, 0, collisionPath});
    }
}
