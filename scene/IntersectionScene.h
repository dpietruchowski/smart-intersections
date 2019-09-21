#ifndef INTERSECTIONSCENE_H
#define INTERSECTIONSCENE_H

#include <map>
#include <QBasicTimer>
#include <QGraphicsScene>
#include <QXmlStreamReader>
#include <QXmlStreamWriter>

#include "Route.h"

class PainterPath;
class PathItem;
class CarItem;
class BaseItem;

class IntersectionScene: public QGraphicsScene
{
public:
    enum class Item {
        PathItem,
        CarItem,
        CollisionAreaItem
    };

    IntersectionScene();

    void reset();
    void start(int msec = 50);
    void stop();

    PathItem* addCarPath(const PainterPath &path,
                            const QPen &pen = QPen(),
                            const QBrush &brush = QBrush());

    PathItem* getPath(int id);
    Route* getRoute(int id);

    Route* createRoute(int id);
    void deleteRoute(int id);

    bool load(QXmlStreamReader& xmlStream);
    void save(QXmlStreamWriter& xmlStream) const;

protected:
    void timerEvent(QTimerEvent *event) override;
    void drawBackground(QPainter *painter, const QRectF &rect) override;
    void onStep();

private:
    BaseItem* createItem(Item item) const;

    template <class BaseItemClass>
    std::vector<BaseItemClass*> getSortedItems() const {
        std::vector<BaseItemClass*> sorted;
        for (QGraphicsItem* item: items()) {
            BaseItemClass* pathItem = dynamic_cast<BaseItemClass*>(item);
            if (pathItem)
                sorted.push_back(pathItem);
        }
        std::sort(sorted.begin(), sorted.end(),
                  [] (BaseItemClass* i1, BaseItemClass* i2) {
            return i1->getId() < i2->getId();
        });
        return sorted;
    }

    void step();
    int getNextId() const;

private:
    QBasicTimer timer_;
    std::map<int, Route> routes_;
    mutable int nextId_ = 0;
};

#endif // INTERSECTIONSCENE_H
