#ifndef CARTIMEAGENT1_H
#define CARTIMEAGENT1_H

#include "CarTimeAgent.h"

class CarTimeAgent1: public CarTimeAgent
{
public:
    using CarTimeAgent::CarTimeAgent;

    void step(int currTime) override;

private:
    void registerFor(size_t id, TimespanAtCollisionArea& taca, int currTime);

private:
    qreal vMax_ = 1000;
};

#endif // CARTIMEAGENT1_H
