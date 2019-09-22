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

    struct TimespanAtCollisionArea {
        int time = 0;
        CollisionPath path;
    };

    void step(int currTime);

    void findCollisionPaths();

    void registerAt(size_t id, int time, CollisionAreaItem *area);

signals:
    void registerMeAt(size_t id, int time, CollisionAreaItem* area);
    void unregisterMeAt(size_t id, int oldtime, int newtime, CollisionAreaItem* area);

private:
    CarItem* car_ = nullptr;
    std::vector<TimespanAtCollisionArea> timespansRegister;
};

#endif // CARAGENT_H
