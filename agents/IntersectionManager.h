#ifndef INTERSECTIONMANAGER_H
#define INTERSECTIONMANAGER_H

#include <map>
#include <QObject>

class CollisionAreaItem;
class CarAgent;

class IntersectionManager: public QObject
{
public:
    static const int TIMELAST = 1000000;
    IntersectionManager();

    void step(int currTime);
    void clear();
    void addCollisionArea(CollisionAreaItem* area);

    void registerTime(size_t id, CarAgent* agent, CollisionAreaItem* area, int time);
    void unregisterTime(CarAgent* agent, CollisionAreaItem* area, int time);

private:
    struct Timespans {
        int time = 0;
        int timespan = 30;
        CarAgent* agent = nullptr;
    };
    using TimespansRegister = std::map<int, Timespans>;

    int getNextAvailableTime(TimespansRegister& timespansRegister, int time);

private:
    std::map<CollisionAreaItem*, TimespansRegister> areas_;
    qreal currTime_ = 0;
};

#endif // INTERSECTIONMANAGER_H
