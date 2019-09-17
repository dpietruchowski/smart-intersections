#include "IntersectionScene.h"

#include <cmath>
#include <QGraphicsItem>
#include <QPainter>
#include <QTimerEvent>
#include "BaseItem.h"
#include "PathItem.h"
#include "CarItem.h"
#include <QDebug>

IntersectionScene::IntersectionScene()
{
}

void IntersectionScene::reset()
{
    for (QGraphicsItem* item: items()) {
        PathItem* path = dynamic_cast<PathItem*>(item);
        if (path) {
            path->reset();
            continue;
        }
    }
    for (QGraphicsItem* item: items()) {
        CarItem* car = dynamic_cast<CarItem*>(item);
        if (car) {
            car->reset();
            car->moveToNextPath(car->getDistance());
            continue;
        }
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
    qDebug() << xmlStream.name();
    if (!res || xmlStream.name() != "intersection")
        return false;

    clear();
    routes_.clear();
    while (!xmlStream.atEnd()) {
        if (!xmlStream.readNextStartElement())
            break;
        if (xmlStream.name() == "roads")
            while (xmlStream.readNextStartElement()) {
                auto* p = new PathItem();
                addItem(p);
                if (!p->load(xmlStream)) {
                    delete p;
                    continue;
                }
            }
        else if (xmlStream.name() == "routes")
            while (xmlStream.readNextStartElement()) {
                int id = xmlStream.attributes().value("id").toInt();
                auto* r = createRoute(id);
                if (r && !r->load(xmlStream)) {
                    deleteRoute(id);
                }
            }
        else if (xmlStream.name() == "cars")
            while (xmlStream.readNextStartElement()) {
                auto* c = new CarItem();
                addItem(c);
                if (!c->load(xmlStream)) {
                    delete c;
                    continue;
                }
            }
        else
            xmlStream.skipCurrentElement();
    }

    return true;
}

void IntersectionScene::save(QXmlStreamWriter& xmlStream) const
{
    xmlStream.writeStartElement("intersection");

    xmlStream.writeStartElement("roads");
    for (QGraphicsItem* item: items()) {
        PathItem* pathItem = dynamic_cast<PathItem*>(item);
        if (pathItem)
            pathItem->save(xmlStream);
    }
    xmlStream.writeEndElement();

    xmlStream.writeStartElement("routes");
    for (auto&[id, route] : routes_) {
        route.save(xmlStream);
    }
    xmlStream.writeEndElement();

    xmlStream.writeStartElement("cars");
    for (QGraphicsItem* item: items()) {
        CarItem* car = dynamic_cast<CarItem*>(item);
        if (car)
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
    for (auto* item : items()) {
        BaseItem* bItem = dynamic_cast<BaseItem*>(item);
        if (bItem)
            bItem->step();
    }
}

int IntersectionScene::getNextId() const
{
    return nextId_++;
}

void IntersectionScene::onStep()
{

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
