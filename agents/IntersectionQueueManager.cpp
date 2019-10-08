#include "IntersectionQueueManager.h"

#include "CarQueueAgent.h"

#include <QDebug>
#include "CarItem.h"

IntersectionQueueManager::IntersectionQueueManager()
{

}

void IntersectionQueueManager::step(int currTime)
{
    CarQueueAgent* current = lastAgents_[currArea_];
    if (!current)
        return;

    current->forward();
    lastAgents_[currArea_] = current->getNextAgent();
}

void IntersectionQueueManager::clear()
{
    lastAgents_.clear();
}

void IntersectionQueueManager::addCollisionArea(CollisionAreaItem* area)
{
    lastAgents_[area] = nullptr;
}

void IntersectionQueueManager::connectCarAgent(CarAgent* agent)
{
    CarQueueAgent* queueAgent = dynamic_cast<CarQueueAgent*>(agent);
    if (!queueAgent)
        return;

    connect(queueAgent, &CarQueueAgent::registerMe,
            [this, queueAgent]  {
        registerCarAgent(queueAgent);
    });
}

void IntersectionQueueManager::setLastAgent(CollisionAreaItem* area, CarQueueAgent* agent)
{
    if (lastAgents_.count(area) == 0)
        return;

    lastAgents_[area] = agent;
}

CarQueueAgent* IntersectionQueueManager::getLastAgent(CollisionAreaItem* area)
{
    if (lastAgents_.count(area) == 0)
        return nullptr;

    return lastAgents_[area];
}

std::vector<CarAgent*> IntersectionQueueManager::getCurrentQueue()
{
    if (!currArea_ || lastAgents_.count(currArea_) == 0)
        return {};
    std::vector<CarAgent*> queue;
    CarQueueAgent* current = lastAgents_[currArea_];
    while (current) {
        queue.push_back(current);
        current = current->getPrevAgent();
    }
    return queue;
}

void IntersectionQueueManager::registerCarAgent(CarQueueAgent* agent)
{
    for (auto& area : lastAgents_) {
        registerForArea(area.first, agent);
    }

}

void IntersectionQueueManager::registerForArea(CollisionAreaItem* area, CarQueueAgent* agent)
{
    CarQueueAgent* prevAgent = lastAgents_[area];

    agent->setPrevAgent(prevAgent);
    agent->setNextAgent(nullptr);

    lastAgents_[area] = agent;
}
