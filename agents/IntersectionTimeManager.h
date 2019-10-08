#ifndef INTERSECTIONTIMEMANAGER_H
#define INTERSECTIONTIMEMANAGER_H

#include <map>
#include "IntersectionManager.h"

class CollisionAreaItem;
class CarTimeAgent;

class IntersectionTimeManager: public IntersectionManager
{
    Q_OBJECT
public:
    static const int TIMELAST = 1000000;
    IntersectionTimeManager();

    void step(int currTime) override;
    void clear() override;
    void addCollisionArea(CollisionAreaItem* area) override;
    void connectCarAgent(CarAgent* agent) override;

    void setTimespan(CarTimeAgent* agent, CollisionAreaItem* area, int time, int timespan);
    void registerTime(size_t id, CarTimeAgent* agent, CollisionAreaItem* area, int time, int timespan);
    void unregisterTime(CarTimeAgent* agent, CollisionAreaItem* area, int time);


signals:
    void timeRegistered(int areaId, int carId, int time, int timespan);
    void timeUnregistered(int areaId, int carId, int time, int timespan);
    void newCollisionArea(int areaId);
    void cleared();

public:
    struct Timespans {
        int time = 0;
        int timespan = 30;
        CarTimeAgent* agent = nullptr;
    };
    // key - time
    using TimespansRegister = std::map<int, Timespans>;
    TimespansRegister* getCurrentRegister();
private:
    int getNextAvailableTime(TimespansRegister& timespansRegister, int time);
    int getNextAvailableTime(TimespansRegister& timespansRegister, int time, int timespan);

private:
    std::map<CollisionAreaItem*, TimespansRegister> areas_;
    qreal currTime_ = 0;
};

#endif // INTERSECTIONTIMEMANAGER_H
