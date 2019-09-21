#ifndef PATHITEM_H
#define PATHITEM_H

#include <memory>
#include <list>
#include <set>
#include <QGraphicsPathItem>
#include "BaseItem.h"
#include "PainterPath.h"

class CarItem;
class CollisionAreaItem;

class PathItem: public QGraphicsPathItem, public BaseItem
{
public:
    explicit PathItem(int id, QGraphicsItem * parent = nullptr);
    explicit PathItem(QGraphicsItem * parent = nullptr);
    ~PathItem() override;

    enum class EntryType {
        In,
        Out,
        Invalid
    };

    struct Entry {
        qreal length = 0;
        EntryType type = EntryType::Invalid;
        //std::set<CollisionAreaItem*> areas;
        CollisionAreaItem* area = nullptr;

        bool isIn() const { return type == EntryType::In; }
        bool isOut() const { return type == EntryType::Out; }
        bool isValid() const {
            if (!area) return false;
            return type != EntryType::Invalid;
        }
    };

    void findEntries();

    void addCar(CarItem* car);
    void removeCar(CarItem* car);

    Entry getNextInEntry(qreal length) const;
    Entry getNextEntry(qreal length) const;

    PainterPath path() const;
    void setPath(const PainterPath &path);

    void paint(QPainter *painter,
               const QStyleOptionGraphicsItem *option,
               QWidget *widget = nullptr) override;

protected:
    void onStep() override;
    void onReset() override;

private:
    void clearCars();
    std::pair<QPointF, qreal> transformAtCar(const CarItem* car);
    qreal percentAtCar(const CarItem* car);
    QPointF pointAtLength(qreal length);
    void updateCar(CarItem* car);

    const char* getItemName() override;
    bool loadItem(QXmlStreamReader& xmlStream) override;
    void saveItem(QXmlStreamWriter& xmlStream) const override;


private:
    std::list<CarItem*> cars_;
    PainterPath path_;
    std::vector<Entry> entries_;

};

#endif // PATHITEM_H
