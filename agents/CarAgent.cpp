#include "CarAgent.h"

#include "CarItem.h"

#include <QDebug>
#include <cmath>
#include "reverse.h"

CarAgent::CarAgent(CarItem* car): car_(car)
{
    defaultVelocity_ = car->getDesiredVelocity();
}

void CarAgent::step(int currTime)
{
    size_t idx = timespansRegister.size() - 1;
    vMax_ = defaultVelocity_;
    for(auto& timespan: reverse(timespansRegister)) {
        if (car_->getRouteDistance(CarItem::Front) < timespan.path.getInDistance()) {
            registerFor(idx, timespan, currTime);
            int endTime = timespansRegister[idx].time + timespansRegister[idx].timespan - 1 - currTime;
            qreal diffDistance = countDiffDistance(timespan);
            qreal areaLength = countAreaLength(timespan);
            qreal distance = diffDistance + areaLength;
            vMax_ = std::min(vMax_, distance / endTime);
        }
        --idx;
    }

    TimespanAtCollisionArea* inTimespan = nullptr;
    for(auto& timespan: timespansRegister) {
        if (car_->getRouteDistance(CarItem::Back) <= timespan.path.getOutDistance()) {
            inTimespan = &timespan;
            break;
        }
    }

    if (car_->isInside() && inTimespan) {
        qreal areaLength = inTimespan->path.getLength() + car_->getLength() + 1;
        qreal velocity = areaLength / (inTimespan->timespan - 1);
        car_->setDesiredVelocity(velocity);
        return;
    }

    TimespanAtCollisionArea* currentTimespan = nullptr;
    for(auto& timespan: timespansRegister) {
        if (car_->getRouteDistance(CarItem::Front) < timespan.path.getInDistance()) {
            currentTimespan = &timespan;
            break;
        }
    }
    if (!currentTimespan) {
        car_->setDesiredVelocity(defaultVelocity_);
        return;
    }

    qreal diffDistance = currentTimespan->path.getInDistance() - car_->getRouteDistance(CarItem::Front) + 0.1;
    qreal diffTime = currentTimespan->time - currTime;
    qreal desiredVelocity = (diffDistance) / diffTime;
    car_->setDesiredVelocity(desiredVelocity);
}

void CarAgent::registerFor(size_t id, TimespanAtCollisionArea& currentTimespan, int currTime)
{
    vMax_ = std::min(vMax_, car_->getMaxVelocity());
    qreal diffDistance = countDiffDistance(currentTimespan);
    qreal areaLength = countAreaLength(currentTimespan);
    qreal velocity = vMax_ + 0.01;
    int time = (diffDistance / velocity) + 1;
    int timespan = (areaLength / velocity) + 1;
    if (currentTimespan.time == 0) {
        emit registerMeAt(id,
                          currTime + time, timespan,
                          currentTimespan.path.getArea());
    } else {
        if (currTime + time < currentTimespan.time) {
            emit unregisterMeAt(id,
                                currentTimespan.time,
                                currTime + time, timespan,
                                currentTimespan.path.getArea());
        }
    }

    qreal diffTime = currentTimespan.time - currTime;
    if (diffTime < 0) {
        car_->setDesiredVelocity(0);
        emit unregisterMeAt(id,
                            currentTimespan.time,
                            currTime + time, timespan,
                            currentTimespan.path.getArea());
    } else if (time > (diffTime + 1)) {
        emit unregisterMeAt(id,
                            currentTimespan.time,
                            currTime + time, timespan,
                            currentTimespan.path.getArea());
    }
}

qreal CarAgent::countDiffDistance(CarAgent::TimespanAtCollisionArea& currentTimespan)
{
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

void CarAgent::registerAt(size_t id, int time, int timespan, CollisionAreaItem* area)
{
    if (id >= timespansRegister.size()) {
        qWarning("Wrong id");
        return;
    }

    if (timespansRegister[id].path.getArea() != area) {
        qWarning("Collision area not match. Registered for other area.");
    }

    timespansRegister[id].time = time;
    timespansRegister[id].timespan = timespan;
}
