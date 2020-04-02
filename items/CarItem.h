#ifndef CARITEM_H
#define CARITEM_H

#include <list>
#include <QGraphicsPolygonItem>
#include "BaseItem.h"
#include "Route.h"

class CollisionAreaItem;
class CarAgent;
class Stat;

class CarItem : public QGraphicsPolygonItem, public BaseItem
{
    friend class CarAgent;
public:
    explicit CarItem(int id, QGraphicsItem * parent = nullptr);
    explicit CarItem(QGraphicsItem * parent = nullptr);
    ~CarItem() override;

    enum CarPart {
        Front,
        Center,
        Back
    };

    void setAgent(CarAgent* agent);

    bool isInside() const;
    void setInside(CollisionAreaItem* area, bool inside);

    qreal getVelocity() const;

    void limitCarVelocity(qreal velocity);
    void setMaxVelocity(qreal velocity);
    qreal getMaxVelocity() const;

    void setDesiredVelocity(qreal velocity);
    qreal getDesiredVelocity() const;

    qreal getDistance(CarPart part = Center) const;
    qreal getRouteDistance(CarPart part = Center) const;
    qreal getNextDistance(CarPart part = Center) const;
    qreal getLength() const;

    void setDefaultDistance(qreal distance);
    qreal getDefaultDistance() const;

    void setRoute(Route* route);
    Route* getRoute();
    PathItem* getNextPath();
    CarItem* getNextCar() const;
    void moveToRouteDistance(qreal routeDistance);
    void moveToNextPath();

    void accept(Stat& Stat, int currentTime) override;

    void paint(QPainter *painter,
               const QStyleOptionGraphicsItem *option,
               QWidget *widget = nullptr) override;

protected:
    void setDistance(qreal distance);

protected:
    void onStep(int currTime) override;
    void onReset() override;

private:
    qreal getDistance(qreal distance, CarPart part = Center) const;
    const char* getItemName() override;
    bool loadItem(QXmlStreamReader& xmlStream) override;
    void saveItem(QXmlStreamWriter& xmlStream) const override;

private:
    CarAgent* agent_ = nullptr;
    std::list<CollisionAreaItem*> areas_;
    qreal desiredVelocity_ = 0;
    qreal maxVelocity_ = 100;
    qreal defaultDistance_ = 0;
    qreal distance_ = 0;
    qreal routeDistance_ = 0;
    Route* route_ = nullptr;
};

#endif // CARITEM_H
