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

void IntersectionManager::registerTime(size_t id, CarAgent* agent, CollisionAreaItem* area, int time)
{
    if (areas_.count(area) == 0) {
        qWarning("This collision area doesn't exist in intersection");
        return;
    }

    auto& timespansRegister = areas_[area];
    int availableTime = getNextAvailableTime(timespansRegister, time);
    timespansRegister[availableTime].time = availableTime;
    timespansRegister[availableTime].timespan = 30;
    timespansRegister[availableTime].agent = agent;

    agent->registerAt(id, availableTime, area);
    emit timeRegistered(area->getId(), agent->getCar()->getId(), availableTime, 30);
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

    timespansRegister.erase(time);
    emit timeUnregistered(area->getId(), agent->getCar()->getId(), time, 10);
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
