#ifndef CARITEM_H
#define CARITEM_H

#include <QGraphicsPolygonItem>
#include "BaseItem.h"

class CarItem: public QGraphicsPolygonItem, public BaseItem
{
public:
    explicit CarItem(QGraphicsItem * parent = nullptr);

    void setVelocity(qreal velocity);
    void setDistance(qreal distance_);

    qreal getVelocity() const;
    qreal getDistance() const;

protected:
    void onStep() override;

private:
    qreal velocity_;
    qreal distance_;
};

#endif // CARITEM_H
