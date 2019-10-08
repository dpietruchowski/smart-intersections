#ifndef CARAGENT_H
#define CARAGENT_H

#include <vector>
#include <QObject>
#include "CollisionPath.h"

class CarItem;

class CarAgent: public QObject
{
public:
    CarAgent(CarItem* car);
    ~CarAgent();

    struct TimespanAtCollisionArea {
        int time = 0;
        int timespan = 0;
        CollisionPath path;
    };

    void setCar(CarItem* car);
    CarItem* getCar() { return car_; }
    const CarItem* getCar() const { return car_; }

    virtual void step(int currTime) = 0;

    void findCollisionPaths();


protected:
    qreal countDiffDistance(TimespanAtCollisionArea& taca);
    qreal countAreaLength(TimespanAtCollisionArea& taca);

protected:
    CarItem* car_ = nullptr;
    qreal defaultVelocity_ = 0;
    std::vector<TimespanAtCollisionArea> timespansRegister;
};

#endif // CARAGENT_H
