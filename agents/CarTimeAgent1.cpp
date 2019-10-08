#include "CarTimeAgent1.h"

#include <cmath>

#include "CarItem.h"
#include "reverse.h"

void CarTimeAgent1::step(int currTime)
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

void CarTimeAgent1::registerFor(size_t id, CarTimeAgent::TimespanAtCollisionArea& currentTimespan, int currTime)
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

    if (car_->getId() == 11)
    {
        int i = 12;
    }
    if (diffTime < 0) {
        car_->setDesiredVelocity(0);
        emit unregisterMeAt(id,
                            currentTimespan.time,
                            currTime + time, timespan,
                            currentTimespan.path.getArea());
    } else if (time > diffTime) {
        emit unregisterMeAt(id,
                            currentTimespan.time,
                            currTime + time, timespan,
                            currentTimespan.path.getArea());
    }
}
