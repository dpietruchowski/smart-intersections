#ifndef CARTIMEAGENT_H
#define CARTIMEAGENT_H

#include "CarAgent.h"

class CarItem;

class CarTimeAgent: public CarAgent
{
    Q_OBJECT
public:
    CarTimeAgent(CarItem* car);
    ~CarTimeAgent();

    void registerAt(size_t id, int time, int timespan, CollisionAreaItem *area);

signals:
    void registerMeAt(size_t id, int time, int timespan, CollisionAreaItem* area);
    void unregisterMeAt(size_t id, int oldtime, int newtime, int timespan, CollisionAreaItem* area);
};

#endif // CARTIMEAGENT_H
