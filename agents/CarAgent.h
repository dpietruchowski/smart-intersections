#ifndef CARAGENT_H
#define CARAGENT_H

#include <vector>
#include <QObject>

#include "CollisionPath.h"

class CarItem;

class CarAgent: public QObject
{
    Q_OBJECT
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

    void step(int currTime);

    void findCollisionPaths();

    void registerAt(size_t id, int time, int timespan, CollisionAreaItem *area);

signals:
    void registerMeAt(size_t id, int time, int timespan, CollisionAreaItem* area);
    void unregisterMeAt(size_t id, int oldtime, int newtime, int timespan, CollisionAreaItem* area);

private:
    void registerFor(size_t id, TimespanAtCollisionArea& taca, int currTime);
    qreal countDiffDistance(TimespanAtCollisionArea& taca);
    qreal countAreaLength(TimespanAtCollisionArea& taca);

private:
    CarItem* car_ = nullptr;
    qreal defaultVelocity_ = 0;
    qreal vMax_ = 1000;
    std::vector<TimespanAtCollisionArea> timespansRegister;
};

#endif // CARAGENT_H
