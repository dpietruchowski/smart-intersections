#ifndef CARITEM_H
#define CARITEM_H

#include <QGraphicsPolygonItem>
#include "BaseItem.h"
#include "Route.h"

class CarItem : public QGraphicsPolygonItem, public BaseItem
{
    friend class CarAgent;
public:
    explicit CarItem(int id, QGraphicsItem * parent = nullptr);
    explicit CarItem(QGraphicsItem * parent = nullptr);

    void limitCarVelocity(qreal velocity);
    void setMaxVelocity(qreal velocity);
    qreal getMaxVelocity() const;

    void setDesiredVelocity(qreal velocity);
    qreal getDesiredVelocity() const;

    qreal getDistance() const;
    qreal getRouteDistance() const;
    qreal getNextDistance() const;

    qreal getFrontDistance() const;
    qreal getNextFrontDistance() const;

    qreal getBackDistance() const;
    qreal getNextBackDistance() const;

    void setDefaultDistance(qreal distance);
    qreal getDefaultDistance() const;


    void setRoute(Route* route);
    PathItem* getNextPath();
    void moveToRouteDistance(qreal routeDistance);
    void moveToNextPath();

protected:
    void setDistance(qreal distance);

protected:
    void onStep() override;
    void onReset() override;

private:
    const char* getItemName() override;
    bool loadItem(QXmlStreamReader& xmlStream) override;
    void saveItem(QXmlStreamWriter& xmlStream) const override;

private:
    qreal desiredVelocity_ = 0;
    qreal maxVelocity_ = 100;
    qreal defaultDistance_ = 0;
    qreal distance_ = 0;
    qreal routeDistance_ = 0;
    Route* route_ = nullptr;
    Route::Iterator routeIter_;
};

#endif // CARITEM_H
