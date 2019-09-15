#ifndef PATHITEM_H
#define PATHITEM_H

#include <memory>
#include <list>
#include <QGraphicsPathItem>
#include "BaseItem.h"
#include "PainterPath.h"
#include "PathNode.h"

class CarItem;

class PathItem: public QGraphicsPathItem, public BaseItem
{
public:
    explicit PathItem(int id, QGraphicsItem * parent = nullptr);
    explicit PathItem(QGraphicsItem * parent = nullptr);
    ~PathItem() override;

    CarItem* addCar(int id, qreal velocity = 0, qreal distance = 0);
    CarItem* addCar(qreal velocity = 0, qreal distance = 0);
    void addCar(CarItem* car);
    void removeCar(CarItem* car);

    bool load(QXmlStreamReader& xmlStream) override;
    void save(QXmlStreamWriter& xmlStream) const override;

    void connectBeginTo(PathNode* node);
    void connectEndTo(PathNode* node);
    bool hasBeginNode() const;
    bool hasEndNode() const;
    void unsetBeginNode();
    void unsetEndNode();
    void setBeginNode(PathNode* node);
    void setEndNode(PathNode* node);

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
    PathNode* beginNode_ = nullptr;
    PathNode* endNode_ = nullptr;

};

#endif // PATHITEM_H
