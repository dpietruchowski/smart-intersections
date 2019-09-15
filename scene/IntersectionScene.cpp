#include "IntersectionScene.h"

#include <cmath>
#include <QGraphicsItem>
#include <QPainter>
#include <QTimerEvent>
#include "BaseItem.h"
#include "PathItem.h"
#include <QDebug>

IntersectionScene::IntersectionScene()
{
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

bool IntersectionScene::load(QXmlStreamReader& xmlStream)
{
    bool res = xmlStream.readNextStartElement();
    qDebug() << xmlStream.name();
    if (!res || xmlStream.name() != "intersection")
        return false;

    clear();
    while (!xmlStream.atEnd()) {
        if (!xmlStream.readNextStartElement())
            break;
        if (xmlStream.name() == "roads")
            while (xmlStream.readNextStartElement()) {
                auto* p = new PathItem();
                if (!p->load(xmlStream)) {
                    delete p;
                    continue;
                }
                addItem(p);
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

    // draw horizontal grid
    qreal startTop = GetStart(rect.top());
    int i = 0;
    for (qreal y = startTop; y <= rect.bottom(); y += step) {
        ++i;
       painter->drawLine(QPointF{rect.left(), y}, QPointF{rect.right(), y});
    }
    // now draw vertical grid
    qreal startLeft = GetStart(rect.left());
    for (qreal x = startLeft; x <= rect.right(); x += step) {
       painter->drawLine(QPointF{x, rect.top()}, QPointF{x, rect.bottom()});
    }
}
