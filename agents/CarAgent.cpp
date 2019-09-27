#include "CarAgent.h"

#include "CarItem.h"

#include <QDebug>

CarAgent::CarAgent(CarItem* car): car_(car)
{
    defaultVelocity_ = car->getDesiredVelocity();
}

void CarAgent::step(int currTime)
{
    TimespanAtCollisionArea* currentTimespan = nullptr;
    size_t id = 0;
    for(auto& timespan: timespansRegister) {
        if (car_->getRouteDistance() < timespan.path.getInDistance()) {
            currentTimespan = &timespan;
            break;
        }
        ++id;
    }

    if (!currentTimespan) {
        car_->setDesiredVelocity(defaultVelocity_);
        return;
    }

    //TODO: vmax

    qreal diffDistance = currentTimespan->path.getInDistance() - car_->getRouteDistance();

    if (currentTimespan->time == 0) {
        emit registerMeAt(id, diffDistance / (car_->getMaxVelocity() + 0.1), currentTimespan->path.getArea());
    } else {
        int newTime = diffDistance / (car_->getMaxVelocity() + 0.1);
        if (newTime < currentTimespan->time) {
            qInfo("ID: %d, new time: %d, max v: %f",
                  car_->getId(), newTime, car_->getMaxVelocity());
            emit unregisterMeAt(id, currentTimespan->time,
                                currTime + newTime, currentTimespan->path.getArea());
        }
    }

    qreal diffTime = currentTimespan->time - currTime;
    if (diffTime < 0) {
        car_->setDesiredVelocity(0);
        int newTime = diffDistance / (car_->getMaxVelocity() + 0.1);
        emit unregisterMeAt(id, currentTimespan->time, currTime + newTime, currentTimespan->path.getArea());
    } else {
        qreal velocity = diffDistance / diffTime;
        car_->setDesiredVelocity(velocity);
    }
}

void CarAgent::findCollisionPaths()
{
    if (!car_)
        return;
    if (!car_->route_)
        return;

    auto collisionPaths = car_->route_->getCollisionPaths();
    for (auto& collisionPath: collisionPaths) {
        timespansRegister.push_back({0, collisionPath});
    }
}

void CarAgent::registerAt(size_t id, int time, CollisionAreaItem* area)
{
    if (id >= timespansRegister.size()) {
        qWarning("Wrong id");
        return;
    }

    if (timespansRegister[id].path.getArea() != area) {
        qWarning("Collision area not match. Registered for other area.");
    }

    timespansRegister[id].time = time;
}
