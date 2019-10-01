#ifndef CARAGENT2_H
#define CARAGENT2_H

#include "CarAgent.h"

class CarAgent2: public CarAgent
{
public:
    using CarAgent::CarAgent;

    void step(int currTime) override;

private:
    void registerFor(size_t id, TimespanAtCollisionArea& taca, int currTime);
};

#endif // CARAGENT2_H
