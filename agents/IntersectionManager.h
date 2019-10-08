#ifndef INTERSECTIONMANAGER_H
#define INTERSECTIONMANAGER_H

#include <QObject>

class CollisionAreaItem;
class CarAgent;

class IntersectionManager: public QObject
{
public:
    IntersectionManager();

    virtual void step(int currTime) = 0;
    virtual void clear() = 0;
    virtual void addCollisionArea(CollisionAreaItem* area) = 0;
    virtual void connectCarAgent(CarAgent* agent) = 0;

    void setCurrentArea(CollisionAreaItem* currArea) { currArea_ = currArea; }

protected:
    CollisionAreaItem* currArea_ = nullptr;
};

#endif // INTERSECTIONMANAGER_H
