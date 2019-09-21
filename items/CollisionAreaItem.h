#ifndef COLLISIONRECTITEM_H
#define COLLISIONRECTITEM_H

#include <QGraphicsRectItem>
#include "BaseItem.h"

class CollisionAreaItem : public QGraphicsRectItem, public BaseItem
{
public:
    explicit CollisionAreaItem(int id, QGraphicsItem * parent = nullptr);
    explicit CollisionAreaItem(QGraphicsItem * parent = nullptr);

    bool containsGlobal(const QPointF point) const;

    bool isOccupied() const { return occupied_; }
    void setOccupied(bool occupied);

protected:
    void onReset() override;

private:
    const char* getItemName() override;
    bool loadItem(QXmlStreamReader& xmlStream) override;
    void saveItem(QXmlStreamWriter& xmlStream) const override;

private:
    bool occupied_ = false;
};

#endif // COLLISIONRECTITEM_H
