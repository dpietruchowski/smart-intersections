#include "IntersectionManager.h"

#include <iterator>

#include "CarAgent.h"
#include "CarItem.h"
#include "CollisionAreaItem.h"

IntersectionManager::IntersectionManager()
{

}

void IntersectionManager::step(int currTime)
{
    currTime_ = currTime;
}

void IntersectionManager::clear()
{
    areas_.clear();
    emit cleared();
    currTime_ = 0;
}

void IntersectionManager::addCollisionArea(CollisionAreaItem* area)
{
    areas_[area];
    emit newCollisionArea(area->getId());
}

void IntersectionManager::setTimespan(CarAgent* agent, CollisionAreaItem* area, int time, int timespan)
{
    if (areas_.count(area) == 0) {
        qWarning("This collision area doesn't exist in intersection");
        return;
    }

    if (timespan > 100)
        timespan = 100;
    auto& timespansRegister = areas_[area];

    if (agent != timespansRegister[time].agent)
        qWarning("Agents not match");

    timespansRegister[time].timespan = timespan;
}

void IntersectionManager::registerTime(size_t id, CarAgent* agent, CollisionAreaItem* area, int time, int timespan)
{
    if (areas_.count(area) == 0) {
        qWarning("This collision area doesn't exist in intersection");
        return;
    }

    if (timespan > 100)
        timespan = 100;
    auto& timespansRegister = areas_[area];
    int availableTime = getNextAvailableTime(timespansRegister, time, timespan);
    timespansRegister[availableTime].time = availableTime;
    timespansRegister[availableTime].timespan = timespan;
    timespansRegister[availableTime].agent = agent;

    agent->registerAt(id, availableTime, timespan, area);
    emit timeRegistered(area->getId(), agent->getCar()->getId(), availableTime, timespan);
}

void IntersectionManager::unregisterTime(CarAgent* agent, CollisionAreaItem* area, int time)
{
    if (areas_.count(area) == 0) {
        qWarning("This collision area doesn't exist in intersection");
        return;
    }

    auto& timespansRegister = areas_[area];

    if (agent != timespansRegister[time].agent)
        qWarning("Agents not match");

    int timespan = timespansRegister[time].timespan;

    timespansRegister.erase(time);
    emit timeUnregistered(area->getId(), agent->getCar()->getId(), time, timespan);
}

IntersectionManager::TimespansRegister* IntersectionManager::getCurrentRegister()
{
    if (!currArea_ || areas_.count(currArea_) == 0)
        return nullptr;

    return &areas_.at(currArea_);
}

int IntersectionManager::getNextAvailableTime(TimespansRegister& timespansRegister, int time)
{
    auto begin = timespansRegister.begin();
    auto end = timespansRegister.end();
    for (auto iter = begin; iter != end; ++iter) {
        int begin = iter->second.time;
        int end = begin + iter->second.timespan;
        if (time < begin)
            return time;
        if (time >= begin && time < end)
            time = end;
    }
    return time;
}

int IntersectionManager::getNextAvailableTime(IntersectionManager::TimespansRegister& timespansRegister, int time, int timespan)
{
    auto begin = timespansRegister.begin();
    auto end = timespansRegister.end();
    auto iter = timespansRegister.lower_bound(time);
    if (iter != begin)
        iter = std::prev(iter);
    for (iter = begin; iter != end; ++iter) {
        int begin = iter->second.time;
        int end = begin + iter->second.timespan;
        auto isBetween = [begin, end] (int time) { return time >= begin && time < end; };
        if (time + timespan <= begin)
            return time;
        else if (isBetween(time) || isBetween(time + timespan))
            time = end;
        else if (time <= begin && time + timespan >= end)
            time = end;
    }
    return time;
    /*
    auto end = timespansRegister.end();

    auto iter = timespansRegister.lower_bound(time);
    auto prev = iter;
    if (iter != timespansRegister.begin())
        prev = std::prev(iter);

    int tBegin = prev->second.time;
    int tEnd = tBegin + prev->second.timespan;
    int tNextBegin = iter->second.time;
    if (time + timespan < tBegin) {
        return time;
    } else if (time > tEnd && time + timespan < tNextBegin) {
        return time;
    }

    for (;iter != end; ++iter) {
        auto next = std::next(iter);
        int tBegin = iter->second.time;
        int tEnd = tBegin + iter->second.timespan;
        if (next != end) {
            int tNextBegin = next->second.time;
            if (tNextBegin - tEnd < 0) qWarning("Diff is lower than 0");
            if (timespan <= tNextBegin - tEnd) {
                return tEnd;
            }
        } else {
            if (time > tEnd) {
                return time;
            } else {
                return tEnd;
            }
        }
    }

    return time;
    */
}
