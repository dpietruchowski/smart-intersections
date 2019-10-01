#ifndef CARAGENT1_H
#define CARAGENT1_H

#include "CarAgent.h"

class CarAgent1: public CarAgent
{
public:
    using CarAgent::CarAgent;

    void step(int currTime) override;

private:
    void registerFor(size_t id, TimespanAtCollisionArea& taca, int currTime);

private:
    qreal vMax_ = 1000;
};

#endif // CARAGENT1_H
