#ifndef INTERSECTIONQUEUEMANAGER_H
#define INTERSECTIONQUEUEMANAGER_H

#include "IntersectionManager.h"

class CarQueueAgent;

class IntersectionQueueManager: public IntersectionManager
{
public:
    IntersectionQueueManager();

    void step(int currTime) override;
    void clear() override;
    void addCollisionArea(CollisionAreaItem* area) override;
    void connectCarAgent(CarAgent* agent) override;

    void setLastAgent(CollisionAreaItem* area, CarQueueAgent* agent);
    CarQueueAgent* getLastAgent(CollisionAreaItem* area);

    std::vector<CarAgent*> getCurrentQueue();

    void registerCarAgent(CarQueueAgent* agent);

private:
    void registerForArea(CollisionAreaItem* area, CarQueueAgent* agent);

private:
    std::map<CollisionAreaItem*, CarQueueAgent*> lastAgents_;
};

#endif // INTERSECTIONQUEUEMANAGER_H
