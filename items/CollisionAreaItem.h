#ifndef COLLISIONRECTITEM_H
#define COLLISIONRECTITEM_H

#include <list>
#include <QGraphicsRectItem>
#include "BaseItem.h"

class CarItem;

class CollisionAreaItem : public QGraphicsRectItem, public BaseItem
{
public:
    explicit CollisionAreaItem(int id, QGraphicsItem * parent = nullptr);
    explicit CollisionAreaItem(QGraphicsItem * parent = nullptr);

    bool containsGlobal(const QPointF point) const;

    bool isOccupied() const { return carsInside_.size() > 0; }
    void setOccupied(CarItem* car, bool occupied = true);

protected:
    void onReset() override;

private:
    const char* getItemName() override;
    bool loadItem(QXmlStreamReader& xmlStream) override;
    void saveItem(QXmlStreamWriter& xmlStream) const override;

private:
    std::list<CarItem*> carsInside_;
    //bool occupied_ = false;
    //int occupiedCount_ = 0;
};

#endif // COLLISIONRECTITEM_H
