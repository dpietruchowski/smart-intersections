#ifndef INTERSECTIONSCENE_H
#define INTERSECTIONSCENE_H

#include <memory>
#include <map>
#include <QBasicTimer>
#include <QGraphicsScene>
#include <QXmlStreamReader>
#include <QXmlStreamWriter>

#include "Route.h"
#include "CarAgent.h"
#include "IntersectionManager.h"
#include "Stat.h"

class PainterPath;
class PathItem;
class CarItem;
class BaseItem;

class IntersectionScene: public QGraphicsScene
{
    Q_OBJECT
public:
    enum class Item {
        PathItem,
        CarItem,
        CollisionAreaItem
    };

    enum Attribute {
        NoAttribute = 0x0000,
        CarPathQueue = 0x0001,
        CollisionAreaBlock = 0x0002
    };
    using Agents = std::vector<std::unique_ptr<CarAgent>>;
    using Stats = std::vector<std::unique_ptr<Stat>>;

    IntersectionScene();

    IntersectionManager* getManager() { return manager_.get(); }
    Agents& getAgents() { return agents_; }
    Stats& getStats() { return stats_; }

    void reset();
    void start(int msec = 10);
    void stop();
    void step();
    void saveStats(const char* filename);
    int getCurrentTime() const { return currentTime_; }

    PathItem* addCarPath(const PainterPath &path,
                            const QPen &pen = QPen(),
                            const QBrush &brush = QBrush());

    PathItem* getPath(int id);
    Route* getRoute(int id);

    Route* createRoute(int id);
    void deleteRoute(int id);

    bool load(QXmlStreamReader& xmlStream);
    void save(QXmlStreamWriter& xmlStream) const;

    bool checkAttribute(Attribute attr) const;

    template <class BaseItemClass>
    std::vector<BaseItemClass*> getItems() const {
        std::vector<BaseItemClass*> baseItems;
        for (QGraphicsItem* item: items()) {
            BaseItemClass* baseItem = dynamic_cast<BaseItemClass*>(item);
            if (baseItem)
                baseItems.push_back(baseItem);
        }
        return baseItems;
    }

    template <class BaseItemClass>
    std::vector<BaseItemClass*> getSortedItems() const {
        std::vector<BaseItemClass*> sorted = getItems<BaseItemClass>();
        std::sort(sorted.begin(), sorted.end(),
                  [] (BaseItemClass* i1, BaseItemClass* i2) {
            return i1->getId() < i2->getId();
        });
        return sorted;
    }

signals:
    void stepped(qreal time);

protected:
    void timerEvent(QTimerEvent *event) override;
    void drawBackground(QPainter *painter, const QRectF &rect) override;

private:
    BaseItem* createItem(Item item) const;
    int getNextId() const;

private:
    QBasicTimer timer_;
    int currentTime_ = 0;
    std::map<int, Route> routes_;

    std::unique_ptr<IntersectionManager> manager_;
    Agents agents_;
    mutable int nextId_ = 0;

    Stats stats_;

    unsigned int attributes_ = NoAttribute;
};

#endif // INTERSECTIONSCENE_H
