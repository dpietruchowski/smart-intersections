#include "IntersectionManager.h"

#include "CarAgent.h"

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
    currTime_ = 0;
}

void IntersectionManager::addCollisionArea(CollisionAreaItem* area)
{
    areas_[area];
}

void IntersectionManager::registerTime(size_t id, CarAgent* agent, CollisionAreaItem* area, int time)
{
    if (areas_.count(area) == 0) {
        qWarning("This collision area doesn't exist in intersection");
        return;
    }


    auto& timespansRegister = areas_[area];
    int availableTime = getNextAvailableTime(timespansRegister, time);
    timespansRegister[availableTime].agent = agent;
    agent->registerAt(id, availableTime, area);
}

void IntersectionManager::unregisterTime(CarAgent* agent, CollisionAreaItem* area, int time)
{
    if (areas_.count(area) == 0) {
        qWarning("This collision area doesn't exist in intersection");
        return;
    }

    auto& timespansRegister = areas_[area];
    timespansRegister[time].timespan = 30;

    if (agent != timespansRegister[time].agent)
        qWarning("Agents not match");
    timespansRegister[time].agent = nullptr;
}

int IntersectionManager::getNextAvailableTime(TimespansRegister& timespansRegister, int time)
{
    for(; time < TIMELAST; time += 40) {
        if (timespansRegister[time].agent == nullptr)
            break;
    }
    return time;
}
