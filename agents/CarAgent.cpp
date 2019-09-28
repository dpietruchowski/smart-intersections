#include "CarAgent.h"

#include "CarItem.h"

#include <QDebug>
#include <cmath>

CarAgent::CarAgent(CarItem* car): car_(car)
{
    defaultVelocity_ = car->getDesiredVelocity();
}

void CarAgent::step(int currTime)
{
    TimespanAtCollisionArea* currentTimespan = nullptr;
    size_t id = 0;
    for(auto& timespan: timespansRegister) {
        if (car_->getRouteDistance(CarItem::Front) < timespan.path.getInDistance()) {
            currentTimespan = &timespan;
            break;
        }
        ++id;
    }

    if (!currentTimespan) {
        qreal defaultVelocity = car_->getMaxVelocity() + 0.01;
        car_->setDesiredVelocity(defaultVelocity);
        return;
    }

    //TODO: vmax

    // 0.1 -- look at PathItem::onPreStep() it is required to limit vmax (just before area)
    qreal diffDistance = currentTimespan->path.getInDistance() - car_->getRouteDistance(CarItem::Front) + 0.1;
    qreal areaLength = currentTimespan->path.getLength() + car_->getLength() + 1;
    qDebug() << areaLength;
    qreal velocity = car_->getMaxVelocity() + 0.01;
    int time = std::ceil(diffDistance / velocity);
    qreal defaultVelocity = std::max(defaultVelocity_, velocity);
    int timespan = std::ceil((areaLength / velocity));
    if (currentTimespan->time == 0) {
        emit registerMeAt(id,
                          currTime + time, timespan,
                          currentTimespan->path.getArea());
    } else {
        if (currTime + time < currentTimespan->time) {
            emit unregisterMeAt(id,
                                currentTimespan->time,
                                currTime + time, timespan,
                                currentTimespan->path.getArea());
        }
    }

    qreal diffTime = currentTimespan->time - currTime;
    if (diffTime < 0) {
        car_->setDesiredVelocity(0);
        emit unregisterMeAt(id,
                            currentTimespan->time,
                            currTime + time, timespan,
                            currentTimespan->path.getArea());
    } else if (time > diffTime) {
        emit unregisterMeAt(id,
                            currentTimespan->time,
                            currTime + time, timespan,
                            currentTimespan->path.getArea());
    }

    diffTime = currentTimespan->time - currTime;
    qreal desiredVelocity = (diffDistance) / diffTime;
    car_->setDesiredVelocity(desiredVelocity);
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
