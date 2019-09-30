#include "CarAgent.h"

#include "CarItem.h"

#include <QDebug>
#include <cmath>
#include "reverse.h"

CarAgent::CarAgent(CarItem* car): car_(car)
{
    car_->setAgent(this);
    defaultVelocity_ = car->getDesiredVelocity();
    vMax_ = defaultVelocity_;
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

void CarAgent::step(int currTime)
{
    vMax_ = defaultVelocity_;
    size_t idx = timespansRegister.size() - 1;
    for(auto& timespan: reverse(timespansRegister)) {
        if (car_->getRouteDistance(CarItem::Front) < timespan.path.getInDistance()) {
            registerFor(idx, timespan, currTime);
            int time = timespan.time - currTime;
            qreal diffDistance = countDiffDistance(timespan);
            if (diffDistance / time < vMax_)
                vMax_ = diffDistance / time;
            //vMax_ = std::min(vMax_, distance / endTime);
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

    TimespanAtCollisionArea* currentTimespan = nullptr;
    for(auto& timespan: timespansRegister) {
        if (car_->getRouteDistance(CarItem::Front) < timespan.path.getInDistance()) {
            currentTimespan = &timespan;
            break;
        }
    }

    if (!currentTimespan) {
        if (car_->isInside() && inTimespan) {
            qreal areaLength = countAreaLength(*inTimespan);
            qreal velocity = areaLength / (inTimespan->timespan - 1);
            velocity = std::min(velocity, defaultVelocity_);
            car_->setDesiredVelocity(velocity);
        } else {
            car_->setDesiredVelocity(defaultVelocity_);
        }
        return;
    }

    qreal diffDistance = countDiffDistance(*currentTimespan);
    int diffTime = currentTimespan->time - currTime;
    qreal desiredVelocity = diffDistance / diffTime;
    desiredVelocity = std::min(desiredVelocity, defaultVelocity_);
    if (car_->isInside() && inTimespan) {
        qreal areaLength = countAreaLength(*inTimespan);
        qreal velocity = areaLength / (inTimespan->timespan - 1);
        velocity = std::min(velocity, defaultVelocity_);
        desiredVelocity = std::max(velocity, desiredVelocity);
    }
    car_->setDesiredVelocity(desiredVelocity);
}

void CarAgent::registerFor(size_t id, TimespanAtCollisionArea& currentTimespan, int currTime)
{
    vMax_ = std::min(vMax_, car_->getMaxVelocity());
    qreal diffDistance = countDiffDistance(currentTimespan);
    qreal areaLength = countAreaLength(currentTimespan);
    qreal velocity = std::max(vMax_, 0.01);

    int time = static_cast<int>(std::floor(diffDistance / velocity));
    int timespan = static_cast<int>(std::ceil(areaLength / velocity) + 1);
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

    int diffTime = currentTimespan.time - currTime;
    if (diffTime < 0) {
        car_->setDesiredVelocity(0);
        emit unregisterMeAt(id,
                            currentTimespan.time,
                            currTime + time, timespan,
                            currentTimespan.path.getArea());
    } else if (time < diffTime) {
        emit unregisterMeAt(id,
                            currentTimespan.time,
                            currTime + time, timespan,
                            currentTimespan.path.getArea());
    }
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
