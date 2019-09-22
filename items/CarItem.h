#ifndef CARITEM_H
#define CARITEM_H

#include <QGraphicsPolygonItem>
#include "BaseItem.h"
#include "Route.h"

class CarItem : public QGraphicsPolygonItem, public BaseItem
{
public:
    explicit CarItem(int id, QGraphicsItem * parent = nullptr);
    explicit CarItem(QGraphicsItem * parent = nullptr);

    void limitCarVelocity(qreal velocity);

    void setVelocity(qreal velocity);
    void setMaxVelocity(qreal velocity);
    void setDesiredVelocity(qreal velocity);
    void setDistance(qreal distance_);
    void setDefaultDistance(qreal distance);

    qreal getVelocity() const;

    qreal getDistance() const;
    qreal getNextDistance() const;

    qreal getFrontDistance() const;
    qreal getNextFrontDistance() const;

    qreal getBackDistance() const;
    qreal getNextBackDistance() const;

    qreal getDefaultDistance() const;


    void setRoute(Route* route);
    PathItem* getNextPath();
    void moveToNextPath(qreal distance = 0);


protected:
    void onStep() override;
    void onReset() override;

private:
    const char* getItemName() override;
    bool loadItem(QXmlStreamReader& xmlStream) override;
    void saveItem(QXmlStreamWriter& xmlStream) const override;

private:
    qreal velocity_ = 0;
    qreal desiredVelocity_ = 0;
    qreal maxVelocity_ = 100;
    qreal defaultDistance_ = 0;
    qreal distance_ = 0;
    Route* route_ = nullptr;
    Route::Iterator routeIter_;
};

#endif // CARITEM_H
