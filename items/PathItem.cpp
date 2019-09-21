#include "PathItem.h"

#include "CarItem.h"

#include <QBrush>
#include <QPen>
#include <QPainter>
#include <QGraphicsScene>
#include <QDebug>

#include "reverse.h"
#include "CollisionAreaItem.h"

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

void PathItem::findEntries()
{
    std::vector<CollisionAreaItem*> areas;
    for(auto* item: scene()->items()) {
        auto* area = dynamic_cast<CollisionAreaItem*>(item);
        if (area) {
            areas.push_back(area);
        }
    }

    entries_.clear();
    bool isInside = false;
    for(qreal l = 0; l < path().length(); ++l) {
        QPointF point = pointAtLength(l);
        bool contains = false;
        Entry entry = {l, EntryType::Invalid, {}};
        for(auto* area: areas) {
            if (area->containsGlobal(point)) {
                contains = true;
                entry.area = area;
                break;
            }
        }

        if (!isInside && contains) {
            isInside = true;
            entry.type = EntryType::In;
            entries_.push_back(entry);
        } else if (isInside && !contains) {
            isInside = false;
            entry.type = EntryType::Out;
            if (entries_.size() > 0)
                entry.area = entries_.back().area;
            entries_.push_back(entry);
        }
    }
}

void PathItem::onReset()
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

PathItem::Entry PathItem::getNextInEntry(qreal length) const
{
    for (auto& entry: entries_) {
        if (length < entry.length && entry.isIn())
            return entry;
    }
    return Entry();
}

PathItem::Entry PathItem::getNextEntry(qreal length) const
{
    for (auto& entry: entries_) {
        if (length < entry.length)
            return entry;
    }
    return Entry();
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

void PathItem::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
    QGraphicsPathItem::paint(painter, option, widget);

    for (auto entry: entries_) {
        QColor color = Qt::red;
        if (entry.type == EntryType::Out)
            color = Qt::blue;
        painter->setBrush(QBrush(color));
        painter->drawEllipse(path_.pointAtLength(entry.length), 3, 3);
    }
}

void PathItem::onStep()
{
    // Limit to the next car
    for(auto iter = cars_.begin(); iter != cars_.end(); ++iter) {
        auto next = std::next(iter);
        qreal vMax = 100;
        if (next != cars_.end()) {
            vMax = (*next)->getDistance() - (*iter)->getDistance() - 50;
        }
        (*iter)->setMaxVelocity(vMax);
    }

    // Limit to the next collision area
    for(auto iter = cars_.begin(); iter != cars_.end(); ++iter) {
        auto entry = getNextInEntry((*iter)->getDistance());
        if (entry.isValid() && entry.isIn() && entry.area->isOccupied()) {
            qreal vMax = entry.length - (*iter)->getDistance() - 25;
            (*iter)->limitCarVelocity(vMax);
            if (vMax < 0)
                qDebug() << "vMAx" << vMax;
        }
    }

    for (auto* car : cars_) {
        updateCar(car);

        auto entry = getNextEntry(car->getPrevDistance());
        if (!entry.isValid() || car->getDistance() < entry.length)
            continue;
        if (entry.isIn()) {
            entry.area->setOccupied(true);
        } else if (entry.isOut()) {
            entry.area->setOccupied(false);
        }
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

void PathItem::updateCar(CarItem* car)
{
    auto positionAngle = transformAtCar(car);
    car->setPos(positionAngle.first);
    car->setRotation(270-positionAngle.second);
}

void PathItem::clearCars()
{
    for(auto* car : cars_) {
        delete car;
    }
    cars_.clear();
}

std::pair<QPointF, qreal> PathItem::transformAtCar(const CarItem* car)
{
    QPainterPath _path = path();
    qreal percent = _path.percentAtLength(car->getDistance());
    qreal angle = _path.angleAtPercent(percent);
    QPointF pos = _path.pointAtPercent(percent);
    pos += this->pos();
    return std::make_pair(pos, angle);
}

qreal PathItem::percentAtCar(const CarItem* car)
{
    return path().percentAtLength(car->getDistance());
}

QPointF PathItem::pointAtLength(qreal length)
{
    return path().pointAtLength(length) + pos();
}

const char* PathItem::getItemName()
{
    return "path";
}


bool PathItem::loadItem(QXmlStreamReader& xmlStream)
{
    qreal x = xmlStream.attributes().value("x").toDouble();
    qreal y = xmlStream.attributes().value("y").toDouble();
    setPos(x, y);

    PainterPath path;
    path.load(xmlStream);
    setPath(path);
    return true;
}

void PathItem::saveItem(QXmlStreamWriter& xmlStream) const
{
    QPointF point = pos();
    xmlStream.writeAttribute("x", QString::number(point.x()));
    xmlStream.writeAttribute("y", QString::number(point.y()));
    path_.save(xmlStream);
}
