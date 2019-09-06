#ifndef PATHITEM_H
#define PATHITEM_H

#include <vector>
#include <QGraphicsPathItem>
#include "BaseItem.h"

class CarItem;

class CarPathItem: public QGraphicsPathItem, public BaseItem
{
public:
    explicit CarPathItem(QGraphicsItem * parent = nullptr);

    CarItem* addCar(qreal velocity = 0, qreal distance = 0);

protected:
    void onStep();

private:
    std::pair<QPointF, qreal> pointAtCar(const CarItem* car);
    qreal percentAtCar(const CarItem* car);
    void updateCar(CarItem* car);

private:
    std::list<CarItem*> cars_;
};

#endif // PATHITEM_H
