#ifndef PATHITEM_H
#define PATHITEM_H

#include <memory>
#include <list>
#include <set>
#include <QGraphicsPathItem>
#include "BaseItem.h"
#include "PainterPath.h"
#include "CollisionPath.h"

class CarItem;
class CollisionAreaItem;

class PathItem: public QGraphicsPathItem, public BaseItem
{
    inline static const qreal MAXIMUM_VELOCITY = 5;
public:
    explicit PathItem(int id, QGraphicsItem * parent = nullptr);
    explicit PathItem(QGraphicsItem * parent = nullptr);
    ~PathItem() override;

    void findCollisionPaths();
    CollisionPath getNextCollisionPath(qreal distance);
    std::vector<CollisionPath> getCollisionPaths() const { return collisionPaths_; }

    void addCar(CarItem* car);
    void removeCar(CarItem* car);

    PainterPath path() const;
    void setPath(const PainterPath &path);

    void paint(QPainter *painter,
               const QStyleOptionGraphicsItem *option,
               QWidget *widget = nullptr) override;

protected:
    void onPreStep() override;
    void onPostStep() override;
    void onReset() override;

private:
    void clearCars();
    std::pair<QPointF, qreal> transformAtCar(const CarItem* car);
    qreal percentAtCar(const CarItem* car);
    QPointF pointAtDistance(qreal distance);
    void updateCar(CarItem* car);

    const char* getItemName() override;
    bool loadItem(QXmlStreamReader& xmlStream) override;
    void saveItem(QXmlStreamWriter& xmlStream) const override;


private:
    std::list<CarItem*> cars_;
    qreal vMax_ = MAXIMUM_VELOCITY;
    PainterPath path_;
    std::vector<CollisionPath> collisionPaths_; // it may be better to use std::list instead because in some case we need to merge it from other path

};

#endif // PATHITEM_H
