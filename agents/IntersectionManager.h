#ifndef INTERSECTIONMANAGER_H
#define INTERSECTIONMANAGER_H

#include <map>
#include <QObject>

class CollisionAreaItem;
class CarAgent;

class IntersectionManager: public QObject
{
    Q_OBJECT
public:
    static const int TIMELAST = 1000000;
    IntersectionManager();

    void step(int currTime);
    void clear();
    void addCollisionArea(CollisionAreaItem* area);

    void setTimespan(CarAgent* agent, CollisionAreaItem* area, int time, int timespan);
    void registerTime(size_t id, CarAgent* agent, CollisionAreaItem* area, int time, int timespan);
    void unregisterTime(CarAgent* agent, CollisionAreaItem* area, int time);


signals:
    void timeRegistered(int areaId, int carId, int time, int timespan);
    void timeUnregistered(int areaId, int carId, int time, int timespan);
    void newCollisionArea(int areaId);
    void cleared();

public:
    struct Timespans {
        int time = 0;
        int timespan = 30;
        CarAgent* agent = nullptr;
    };
    using TimespansRegister = std::map<int, Timespans>;
    void setCurrentArea(CollisionAreaItem* currArea) { currArea_ = currArea; }
    TimespansRegister* getCurrentRegister();
private:
    int getNextAvailableTime(TimespansRegister& timespansRegister, int time);
    int getNextAvailableTime(TimespansRegister& timespansRegister, int time, int timespan);

private:
    std::map<CollisionAreaItem*, TimespansRegister> areas_;
    CollisionAreaItem* currArea_ = nullptr;
    qreal currTime_ = 0;
};

#endif // INTERSECTIONMANAGER_H
