#include "IntersectionScene.h"

#include <cmath>
#include <QGraphicsItem>
#include <QPainter>
#include <QTimerEvent>
#include "BaseItem.h"
#include "PathItem.h"
#include "CarItem.h"
#include "CollisionAreaItem.h"
#include <QDebug>

IntersectionScene::IntersectionScene()
{
}

void IntersectionScene::reset()
{
    for (auto* baseItem: getSortedItems<BaseItem>()) {
        baseItem->reset();
    }
    /*
    std::vector<CarItem*> cars = getSortedItems<CarItem>();
    for (auto* car: cars) {
        car->moveToNextPath();
    }

    std::sort(cars.begin(), cars.end(), [this] (CarItem* a, CarItem* b) {
        return a->getDefaultDistance()> b->getDefaultDistance();
    });

    qreal i = 0.1;
    qreal factor = 0.1;
    qreal d = 100;
    for(CarItem* car: cars) {
        d = 100 - car->getDefaultDistance()/10;
        qreal y = d/i;
        qreal x = y*0.1125;
        car->setVelocity(x);
        factor *= 1.25;
        if (factor > 0.25)
            factor = 0.25;
        i += factor;
    }*/
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
    if (!res || xmlStream.name() != "intersection")
        return false;

    clear();
    routes_.clear();

    qreal x = xmlStream.attributes().value("x").toDouble();
    qreal y = xmlStream.attributes().value("y").toDouble();
    qreal w = xmlStream.attributes().value("w").toDouble();
    qreal h = xmlStream.attributes().value("h").toDouble();

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
    onStep();
    for (auto* car: getItems<BaseItem>()) {
        car->prestep();
    }
    for (auto* car: getItems<BaseItem>()) {
        car->step();
    }
    for (auto* car: getItems<BaseItem>()) {
        car->poststep();
    }
}

int IntersectionScene::getNextId() const
{
    return nextId_++;
}

void IntersectionScene::onStep()
{

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
