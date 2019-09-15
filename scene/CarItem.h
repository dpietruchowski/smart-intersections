#ifndef CARITEM_H
#define CARITEM_H

#include <QGraphicsPolygonItem>
#include "BaseItem.h"
#include "Route.h"

class CarItem: public QGraphicsPolygonItem, public BaseItem
{
public:
    explicit CarItem(int id, QGraphicsItem * parent = nullptr);
    explicit CarItem(QGraphicsItem * parent = nullptr);

    void reset();

    void setVelocity(qreal velocity);
    void setDistance(qreal distance_);
    void setDefaultDistance(qreal distance);

    qreal getVelocity() const;
    qreal getDistance() const;
    qreal getDefaultDistance() const;

    void setRoute(Route* route);
    PathItem* getNextPath();
    void moveToNextPath(qreal distance = 0);

    bool load(QXmlStreamReader& xmlStream) override;
    void save(QXmlStreamWriter& xmlStream) const override;

protected:
    void onStep() override;

private:
    qreal velocity_ = 0;
    qreal defaultDistance_ = 0;
    qreal distance_ = 0;
    Route* route_ = nullptr;
    Route::Iterator routeIter_;
};

#endif // CARITEM_H
