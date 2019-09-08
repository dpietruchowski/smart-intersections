#include "IntersectionScene.h"

#include <cmath>
#include <QGraphicsItem>
#include <QPainter>
#include <QTimerEvent>
#include "BaseItem.h"
#include "CarPathItem.h"
#include <QDebug>

IntersectionScene::IntersectionScene()
{
    auto* p = new CarPathItem();
    addItem(p);
}

void IntersectionScene::start(int msec)
{
    timer_.start(msec, this);
}

void IntersectionScene::stop()
{
    timer_.stop();
}

CarPathItem* IntersectionScene::addCarPath(const PainterPath& path, const QPen& pen, const QBrush& brush)
{
    auto* carPath = new CarPathItem();
    carPath->setPath(path);
    carPath->setPen(pen);
    carPath->setBrush(brush);

    addItem(carPath);
    return carPath;
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
        auto* p = new CarPathItem();
        if (!p->load(xmlStream)) {
            delete p;
            continue;
        }
        addItem(p);
    }
    return true;
}

void IntersectionScene::save(QXmlStreamWriter& xmlStream) const
{
    xmlStream.writeStartElement("intersection");
    for (QGraphicsItem* item: items()) {
        CarPathItem* pathItem = dynamic_cast<CarPathItem*>(item);
        if (pathItem)
            pathItem->save(xmlStream);
    }
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
