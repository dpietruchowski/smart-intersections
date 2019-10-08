#ifndef CARTIMEAGENT2_H
#define CARTIMEAGENT2_H

#include "CarTimeAgent.h"

class CarTimeAgent2: public CarTimeAgent
{
public:
    using CarTimeAgent::CarTimeAgent;

    void step(int currTime) override;

private:
    void registerFor(size_t id, TimespanAtCollisionArea& taca, int currTime);
};

#endif // CARTIMEAGENT2_H
