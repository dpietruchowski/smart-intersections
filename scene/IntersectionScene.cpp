#include "IntersectionScene.h"

#include <cmath>
#include <QGraphicsItem>
#include <QPainter>
#include <QTimerEvent>
#include "BaseItem.h"
#include "PathItem.h"
#include "CarItem.h"
#include "CollisionAreaItem.h"

#include "CarAgent1.h"
#include <QDebug>

#include "Stat.h"
#include "CarVelocityStat.h"
#include "CarDistanceStat.h"

IntersectionScene::IntersectionScene()
{
    connect(this, &IntersectionScene::focusItemChanged,
            [this] (QGraphicsItem* newFocusItem, QGraphicsItem* oldFocusItem, Qt::FocusReason reason) {
        CollisionAreaItem* newArea = dynamic_cast<CollisionAreaItem*>(newFocusItem);
        if (newArea)
            manager_.setCurrentArea(newArea);
        else
            manager_.setCurrentArea(nullptr);
    });
    stats_.push_back(std::make_unique<CarVelocityStat>());
    stats_.push_back(std::make_unique<CarDistanceStat>());
}

void IntersectionScene::reset()
{
    qDebug() << "reset";
    currentTime_ = 0;
    for (auto* baseItem: getSortedItems<CollisionAreaItem>()) {
        baseItem->reset();
    }
    for (auto* baseItem: getSortedItems<PathItem>()) {
        baseItem->reset();
    }
    for (auto* baseItem: getSortedItems<CarItem>()) {
        baseItem->reset();
    }

    agents_.clear();
    manager_.clear();

    for(auto& stat: stats_) {
        stat->clear();
    }

    for (auto* area: getItems<CollisionAreaItem>()) {
        manager_.addCollisionArea(area);
    }

    auto cars = getItems<CarItem>();
    std::sort(cars.begin(), cars.end(), [](const CarItem* a, const CarItem* b) {
        return a->getDistance() > b->getDistance();
    });
    for (auto* car: cars) {
        auto& agent = agents_.emplace_back(std::make_unique<CarAgent1>(car));
        agent->findCollisionPaths();
    }

    for (auto& agent: agents_) {
        CarAgent* carAgent = agent.get();
        connect(carAgent, &CarAgent::registerMeAt,
                [this, carAgent] (size_t id, int time, int timespan, CollisionAreaItem* area) {
            manager_.registerTime(id, carAgent, area, time, timespan);
        });
        connect(carAgent, &CarAgent::unregisterMeAt,
                [this, carAgent] (size_t id, int oldTime, int time, int timespan, CollisionAreaItem* area) {
            manager_.unregisterTime(carAgent, area, oldTime);
            manager_.registerTime(id, carAgent, area, time, timespan);
        });
    }
}

void IntersectionScene::start(int msec)
{
    timer_.start(msec, this);
}

void IntersectionScene::stop()
{
    timer_.stop();
}

PathItem* IntersectionScene::addCarPath(const PainterPath& path, const QPen& pen, const QBrush& brush)
{
    auto* carPath = new PathItem(getNextId());
    carPath->setPath(path);
    carPath->setPen(pen);
    carPath->setBrush(brush);

    addItem(carPath);
    return carPath;
}

PathItem* IntersectionScene::getPath(int id)
{
    for (QGraphicsItem* item: items()) {
        PathItem* pathItem = dynamic_cast<PathItem*>(item);
        if (pathItem && pathItem->getId() == id)
            return pathItem;
    }
    return nullptr;
}

Route* IntersectionScene::getRoute(int id)
{
    if (routes_.count(id) == 0)
        return nullptr;

    return &routes_.at(id);
}

Route* IntersectionScene::createRoute(int id)
{
    auto res = routes_.try_emplace(id, id, *this);
    if (!res.second)
        return nullptr;

    Route& route = (*res.first).second;
    return &route;
}

void IntersectionScene::deleteRoute(int id)
{
    if (routes_.count(id) == 0)
        return;

    routes_.erase(id);
}


bool IntersectionScene::load(QXmlStreamReader& xmlStream)
{
    bool res = xmlStream.readNextStartElement();
    if (!res) qWarning("%s", xmlStream.errorString().toStdString().c_str());
    if (!res || xmlStream.name() != "intersection")
        return false;

    clear();
    qDebug() << "II" << items().size();
    attributes_ = NoAttribute;
    routes_.clear();

    qreal x = xmlStream.attributes().value("x").toDouble();
    qreal y = xmlStream.attributes().value("y").toDouble();
    qreal w = xmlStream.attributes().value("w").toDouble();
    qreal h = xmlStream.attributes().value("h").toDouble();

    if (xmlStream.attributes().hasAttribute("car-path-queue"))
        attributes_ |= CarPathQueue;

    if (xmlStream.attributes().hasAttribute("collision-area-block"))
        attributes_ |= CollisionAreaBlock;

    setSceneRect(x, y, w, h);

    auto loadGraphicsItems = [this] (QXmlStreamReader& xmlStream, Item itemClass) {
        while (xmlStream.readNextStartElement()) {
            BaseItem* item = createItem(itemClass);
            addItem(item->getItem<QGraphicsItem>());
            if (!item->load(xmlStream)) {
                delete item;
                continue;
            }
        }
    };

    while (!xmlStream.atEnd()) {
        if (!xmlStream.readNextStartElement())
            break;
        if (xmlStream.name() == "roads")
            loadGraphicsItems(xmlStream, Item::PathItem);
        else if (xmlStream.name() == "routes")
            while (xmlStream.readNextStartElement()) {
                int id = xmlStream.attributes().value("id").toInt();
                auto* r = createRoute(id);
                if (r && !r->load(xmlStream)) {
                    deleteRoute(id);
                }
            }
        else if (xmlStream.name() == "cars")
            loadGraphicsItems(xmlStream, Item::CarItem);
        else if (xmlStream.name() == "collision-areas")
            loadGraphicsItems(xmlStream, Item::CollisionAreaItem);
        else
            xmlStream.skipCurrentElement();
    }

    for (auto* pathItem: getSortedItems<PathItem>()) {
        pathItem->findCollisionPaths();
    }

    return true;
}

void IntersectionScene::save(QXmlStreamWriter& xmlStream) const
{
    xmlStream.writeStartElement("intersection");

    QRectF rect = sceneRect();
    xmlStream.writeAttribute("x", QString::number(rect.x()));
    xmlStream.writeAttribute("y", QString::number(rect.y()));
    xmlStream.writeAttribute("w", QString::number(rect.width()));
    xmlStream.writeAttribute("h", QString::number(rect.height()));

    if (checkAttribute(CarPathQueue))
        xmlStream.writeAttribute("car-path-queue", "");
    if (checkAttribute(CollisionAreaBlock))
        xmlStream.writeAttribute("collision-area-block", "");

    xmlStream.writeStartElement("roads");
    for (PathItem* pathItem: getSortedItems<PathItem>()) {
        pathItem->save(xmlStream);
    }
    xmlStream.writeEndElement();

    xmlStream.writeStartElement("collision-areas");
    for (CollisionAreaItem* area: getSortedItems<CollisionAreaItem>()) {
        area->save(xmlStream);
    }
    xmlStream.writeEndElement();

    xmlStream.writeStartElement("routes");
    for (auto&[id, route] : routes_) {
        route.save(xmlStream);
    }
    xmlStream.writeEndElement();

    xmlStream.writeStartElement("cars");
    for (CarItem* car: getSortedItems<CarItem>()) {
        car->save(xmlStream);
    }
    xmlStream.writeEndElement();

    xmlStream.writeEndElement();
}

bool IntersectionScene::checkAttribute(Attribute attr) const
{
    return attributes_ & attr;
}

void IntersectionScene::timerEvent(QTimerEvent* event)
{
    if (event->timerId() == timer_.timerId()) {
        step();
    } else {
        QGraphicsScene::timerEvent(event);
    }
}

void IntersectionScene::step()
{
    qDebug() << "step";
    auto paths = getItems<PathItem>();
    for (auto* path: getItems<PathItem>()) {
        path->step(currentTime_);
    }
    for (auto* item: getItems<BaseItem>()) {
        for(auto& stat: stats_) {
            item->accept(*stat, currentTime_);
        }
    }
    ++currentTime_;
    emit stepped(currentTime_);
}

void IntersectionScene::refresh()
{
    for (auto& stat: stats_) {
        stat->refresh();
    }
}

int IntersectionScene::getNextId() const
{
    return nextId_++;
}

BaseItem* IntersectionScene::createItem(IntersectionScene::Item item) const
{
    switch (item) {
        case Item::CarItem: return new CarItem();
        case Item::PathItem: return new PathItem();
        case Item::CollisionAreaItem: return new CollisionAreaItem();
    }
    return nullptr;
}

void IntersectionScene::drawBackground(QPainter* painter, const QRectF& rect)
{
    int step = 10;
    painter->setPen(QPen(QColor(200, 200, 255, 125)));
    auto GetStart = [step](qreal edge) {
        double mod = std::fmod(edge, step);
        if (edge < 0 || mod == 0.0) {
            return edge - mod;
        } else {
            return edge - mod + step;
        }
    };

    auto DrawLine = [painter] (const QPointF &p1, const QPointF &p2, qreal lineWidth) {
        painter->save();
        QPen pen = painter->pen();
        pen.setWidthF(lineWidth);
        painter->setPen(pen);
        painter->drawLine(p1, p2);
        painter->restore();
    };

    // draw horizontal grid
    qreal startTop = GetStart(rect.top());
    for (qreal y = startTop; y <= rect.bottom(); y += step) {
        if (y == 0.0)
            DrawLine(QPointF{rect.left(), y}, QPointF{rect.right(), y}, 3);
        else
            painter->drawLine(QPointF{rect.left(), y}, QPointF{rect.right(), y});
    }
    // now draw vertical grid
    qreal startLeft = GetStart(rect.left());
    for (qreal x = startLeft; x <= rect.right(); x += step) {
        if (x == 0.0)
            DrawLine(QPointF{x, rect.top()}, QPointF{x, rect.bottom()}, 3);
        else
            painter->drawLine(QPointF{x, rect.top()}, QPointF{x, rect.bottom()});
    }
}
