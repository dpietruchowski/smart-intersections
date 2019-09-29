#ifndef CARITEM_H
#define CARITEM_H

#include <list>
#include <QGraphicsPolygonItem>
#include "BaseItem.h"
#include "Route.h"

class CollisionAreaItem;

class CarItem : public QGraphicsPolygonItem, public BaseItem
{
    friend class CarAgent;
public:
    explicit CarItem(int id, QGraphicsItem * parent = nullptr);
    explicit CarItem(QGraphicsItem * parent = nullptr);

    enum CarPart {
        Front,
        Center,
        Back
    };

    bool isInside() const;
    void setInside(CollisionAreaItem* area, bool inside);

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
    PathItem* getNextPath();
    void moveToRouteDistance(qreal routeDistance);
    void moveToNextPath();

    void paint(QPainter *painter,
               const QStyleOptionGraphicsItem *option,
               QWidget *widget = nullptr) override;

protected:
    void setDistance(qreal distance);

protected:
    void onStep() override;
    void onReset() override;

private:
    qreal getDistance(qreal distance, CarPart part = Center) const;
    const char* getItemName() override;
    bool loadItem(QXmlStreamReader& xmlStream) override;
    void saveItem(QXmlStreamWriter& xmlStream) const override;

private:
    std::list<CollisionAreaItem*> areas_;
    qreal desiredVelocity_ = 0;
    qreal maxVelocity_ = 100;
    qreal defaultDistance_ = 0;
    qreal distance_ = 0;
    qreal routeDistance_ = 0;
    Route* route_ = nullptr;
    Route::Iterator routeIter_;
};

#endif // CARITEM_H
