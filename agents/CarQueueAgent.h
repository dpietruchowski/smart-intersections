#ifndef CARQUEUEAGENT_H
#define CARQUEUEAGENT_H

#include "CarAgent.h"

class CarQueueAgent: public CarAgent
{
    Q_OBJECT
public:
    CarQueueAgent(CarItem* car);

    void step(int currTime) override;

    void setPrevAgent(CarQueueAgent* agent);
    void setNextAgent(CarQueueAgent* agent);

    CarQueueAgent* getPrevAgent() { return prevAgent_; }
    CarQueueAgent* getNextAgent() { return nextAgent_; }

    void forward();

signals:
    void registerMe();

private:
    CarQueueAgent* prevAgent_ = nullptr;
    CarQueueAgent* nextAgent_ = nullptr;
};

#endif // CARQUEUEAGENT_H
