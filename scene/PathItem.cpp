#include "PathItem.h"

#include "CarItem.h"

#include <QBrush>
#include <QPen>
#include <QDebug>

PathItem::PathItem(int id, QGraphicsItem* parent): QGraphicsPathItem(parent), BaseItem(id)
{
    PainterPath path;
    path.moveTo(0, 0);
    path.quadTo(10, 20, 20, 20);
    path.lineTo(10, 10);
    setPath(path);
}

PathItem::PathItem(QGraphicsItem* parent): PathItem(-1, parent)
{

}

PathItem::~PathItem()
{
}

void PathItem::reset()
{
    cars_.clear();
}


void PathItem::addCar(CarItem* car)
{
    updateCar(car);
    cars_.push_back(car);

    cars_.sort([](CarItem* a, CarItem* b){
        return a->getDistance() < b->getDistance();
    });
}

void PathItem::removeCar(CarItem* car)
{
    cars_.remove(car);
}

bool PathItem::load(QXmlStreamReader& xmlStream)
{
    if (!xmlStream.isStartElement() ||
            xmlStream.name() != "path")
        return false;

    int id = xmlStream.attributes().value("id").toInt();
    qreal x = xmlStream.attributes().value("x").toDouble();
    qreal y = xmlStream.attributes().value("y").toDouble();
    setId(id);
    setPos(x, y);

    if (xmlStream.attributes().hasAttribute("color")) {
        QColor color = xmlStream.attributes().value("color").toString();
        QPen p = pen();
        p.setWidth(2);
        p.setColor(color);
        setPen(p);
    }

    PainterPath path;
    path.load(xmlStream);
    setPath(path);
    return true;
}

void PathItem::save(QXmlStreamWriter& xmlStream) const
{
    xmlStream.writeStartElement("path");
    QPointF point = pos();
    xmlStream.writeAttribute("id", QString::number(getId()));
    xmlStream.writeAttribute("x", QString::number(point.x()));
    xmlStream.writeAttribute("y", QString::number(point.y()));
    QPen p = pen();
    xmlStream.writeAttribute("color", p.color().name());
    path_.save(xmlStream);
    xmlStream.writeEndElement();
}

PainterPath PathItem::path() const
{
    return path_;
}

void PathItem::setPath(const PainterPath& path)
{
    path_ = path;
    QGraphicsPathItem::setPath(path);
}

void PathItem::onStep()
{
    if ((*cars_.rbegin())->getDistance() > 500)
        (*cars_.rbegin())->setVelocity(5);
    for(auto iter = cars_.begin(); iter != cars_.end(); ++iter) {
        auto next = std::next(iter);
        qreal vMax = 100;
        if (next != cars_.end()) {
            vMax = (*next)->getDistance() - (*iter)->getDistance() - 50;
        }
        (*iter)->setMaxVelocity(vMax);
    }

    for (auto* car : cars_) {
        updateCar(car);
    }

    std::vector<CarItem*> removed;
    cars_.remove_if([this, &removed] (CarItem* car) {
        if (percentAtCar(car) >= 1) {
            removed.push_back(car);
            return true;
        }
        return false;
    });

    for (auto* car : removed) {
        if (percentAtCar(car) >= 1)
            car->moveToNextPath();
    }
}

void PathItem::clearCars()
{
    for(auto* car : cars_) {
        delete car;
    }
    cars_.clear();
}

std::pair<QPointF, qreal> PathItem::pointAtCar(const CarItem* car)
{
    QPainterPath _path = path();
    qreal percent = _path.percentAtLength(car->getDistance());
    QPointF pos = _path.pointAtPercent(percent);
    pos += this->pos();
    qreal angle = _path.angleAtPercent(percent);
    return std::make_pair(pos, angle);
}

qreal PathItem::percentAtCar(const CarItem* car)
{
    return path().percentAtLength(car->getDistance());
}

void PathItem::updateCar(CarItem* car)
{
    auto positionAngle = pointAtCar(car);
    car->setPos(positionAngle.first);
    car->setRotation(270-positionAngle.second);
}
