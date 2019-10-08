#include "CarQueueAgent.h"

CarQueueAgent::CarQueueAgent(CarItem* car): CarAgent(car)
{
}

void CarQueueAgent::step(int currTime)
{
    if (currTime == 0)
        emit registerMe();


}

void CarQueueAgent::setPrevAgent(CarQueueAgent* agent)
{
    prevAgent_ = agent;
    if (agent)
        agent->nextAgent_ = this;
}

void CarQueueAgent::setNextAgent(CarQueueAgent* agent)
{
    nextAgent_ = agent;
    if (agent)
        agent->prevAgent_ = this;
}

void CarQueueAgent::forward()
{
    if (!prevAgent_)
        return;

    CarQueueAgent* prevAgent = prevAgent_;
    prevAgent_->setNextAgent(nextAgent_);

    setPrevAgent(prevAgent->prevAgent_);
    setNextAgent(prevAgent);
}
