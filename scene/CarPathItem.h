#ifndef PATHITEM_H
#define PATHITEM_H

#include <vector>
#include <QGraphicsPathItem>
#include "BaseItem.h"
#include "PainterPath.h"

class CarItem;

class CarPathItem: public QGraphicsPathItem, public BaseItem
{
public:
    explicit CarPathItem(QGraphicsItem * parent = nullptr);

    CarItem* addCar(qreal velocity = 0, qreal distance = 0);

    bool load(QXmlStreamReader& xmlStream) override;
    void save(QXmlStreamWriter& xmlStream) const override;

    PainterPath path() const;
    void setPath(const PainterPath &path);

protected:
    void onStep();

private:
    void clearCars();
    bool loadCars(QXmlStreamReader& xmlStream);
    std::pair<QPointF, qreal> pointAtCar(const CarItem* car);
    qreal percentAtCar(const CarItem* car);
    void updateCar(CarItem* car);

private:
    std::list<CarItem*> cars_;
    PainterPath path_;
};

#endif // PATHITEM_H
