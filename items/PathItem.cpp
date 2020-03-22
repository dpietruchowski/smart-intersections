#include "PathItem.h"

#include "CarItem.h"

#include <QBrush>
#include <QPen>
#include <QPainter>
#include <QGraphicsScene>
#include <QDebug>

#include "reverse.h"
#include "CollisionAreaItem.h"
#include "IntersectionScene.h"

PathItem::PathItem(int id, QGraphicsItem* parent): QGraphicsPathItem(parent), BaseItem(id)
{
    PainterPath path;
    path.moveTo(0, 0);
    path.quadTo(10, 20, 20, 20);
    path.lineTo(10, 10);
    setPath(path);
    setToolTip(QString::number(id));
}

PathItem::PathItem(QGraphicsItem* parent): PathItem(-1, parent)
{

}

PathItem::~PathItem()
{
}

void PathItem::findCollisionPaths()
{
    for(auto* area: getIntersection()->getItems<CollisionAreaItem>()) {
        bool isInside = false;
        qreal inDistance = 0;
        for (qreal l = 0; l < path().length(); ++l) {
            QPointF point = pointAtDistance(l);
            bool contains = area->containsGlobal(point);

            if (!isInside && contains) {
                isInside = true;
                inDistance = l;
            } else if (isInside && !contains) {
                isInside = false;
                qreal outDistance = l;
                collisionPaths_.emplace_back(inDistance, outDistance, area);
            }
        }
        if (isInside)
            collisionPaths_.emplace_back(inDistance, path().length(), area);
    }

    std::sort(collisionPaths_.begin(), collisionPaths_.end(),
              [] (CollisionPath& p1, CollisionPath& p2) {
        return p1.getInDistance() < p2.getInDistance();
    });
}

CollisionPath PathItem::getNextCollisionPath(qreal distance)
{
    for (auto& collisionPath: collisionPaths_) {
        if (collisionPath.getInDistance() > distance)
            return collisionPath;
    }
    return CollisionPath{};
}

qreal PathItem::getFirstCarDistance() const
{
    auto firstCar = cars_.begin();
    if (firstCar == cars_.end())
        return path_.length();

    CarItem* car = *firstCar;
    return car->getDistance(CarItem::Back);
}

CarItem* PathItem::getNextCar(qreal distance)
{
    auto ret = std::find_if(cars_.begin(), cars_.end(),
                            [distance] (CarItem* car) {
        return car->getDistance() > distance;
    });

    if (ret != cars_.end())
        return *ret;

    return nullptr;
}

void PathItem::onReset()
{
    cars_.clear();
}


void PathItem::addCar(CarItem* car)
{
    if (std::find(cars_.begin(), cars_.end(), car) != cars_.end())
        return;

    updateCar(car);
    cars_.push_back(car);

    for (auto cPath: collisionPaths_) {
        bool isInside = cPath.isPartInside(car->getDistance(CarItem::Back),
                                           car->getDistance(CarItem::Front));

        if (isInside) {
            cPath.getArea()->setOccupied(car, true);
        }
    }
    cars_.sort([](CarItem* a, CarItem* b){
        return a->getDistance() < b->getDistance();
    });
}

void PathItem::removeCar(CarItem* car)
{
    cars_.remove(car);
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

void PathItem::accept(Stat& Stat, int currentTime)
{
    Stat.visit(*this, currentTime);
}

void PathItem::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
    QGraphicsPathItem::paint(painter, option, widget);

    for (auto collisionPath: collisionPaths_) {
        painter->setBrush(QBrush(Qt::red));
        painter->drawEllipse(path_.pointAtLength(collisionPath.getInDistance()), 3, 3);
        painter->setBrush(QBrush(Qt::blue));
        painter->drawEllipse(path_.pointAtLength(collisionPath.getOutDistance()), 3, 3);
    }
}

void PathItem::onPreStep()
{
    IntersectionScene* intersection = getIntersection();
    // Limit to the next car
    for(auto iter = cars_.begin(); iter != cars_.end(); ++iter) {
        auto next = std::next(iter);
        qreal vMax = vMax_;
        if (next != cars_.end()
                && intersection->checkAttribute(IntersectionScene::CarPathQueue)) {
            qreal distance = (*next)->getDistance(CarItem::Back) - (*iter)->getDistance(CarItem::Front) - 2;
            if (distance < vMax)
                vMax = distance;
        }
        (*iter)->setMaxVelocity(vMax);
    }

    auto lastCar = cars_.rbegin();
    if (lastCar == cars_.rend())
        return;

    CarItem* car = (*lastCar)->getNextCar();
    if (!car)
        return;

    qreal distance = car->getRouteDistance(CarItem::Back) - (*lastCar)->getRouteDistance(CarItem::Front) - 2;
    (*lastCar)->limitCarVelocity(distance);
}

void PathItem::onStep(int currTime)
{
    onPreStep();

    IntersectionScene* intersection = getIntersection();
    // Limit to the next collision area
    for(auto iter = cars_.rbegin(); iter != cars_.rend(); ++iter) {
        auto* car = *iter;
        auto collisionPath = getNextCollisionPath(car->getDistance(CarItem::Front));
        if (collisionPath.isValid()) {
            if (collisionPath.getArea()->isOccupied()
                    && intersection->checkAttribute(IntersectionScene::CollisionAreaBlock)) {
                qreal vMax = collisionPath.getInDistance() - car->getDistance(CarItem::Front) - 0.1;
                car->limitCarVelocity(vMax);
            }
        }
        car->step(currTime);

        for (auto cPath: collisionPaths_) {
            bool isInsideArea = cPath.isPartInside(car->getDistance(CarItem::Back),
                                               car->getDistance(CarItem::Front));
            if (isInsideArea)
                cPath.getArea()->setOccupied(car, true);
            else
                cPath.getArea()->setOccupied(car, false);
        }
    }

    onPostStep();
}

void PathItem::onPostStep()
{
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
        for (auto cPath: collisionPaths_) {
            bool isInside = cPath.isPartInside(car->getDistance(CarItem::Back),
                                               car->getDistance(CarItem::Front));

            if (isInside) {
                cPath.getArea()->setOccupied(car, false);
            }
        }
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

std::pair<QPointF, qreal> PathItem::transformAtCar(const CarItem* car) const
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

QPointF PathItem::pointAtDistance(qreal distance)
{
    return path().pointAtLength(distance) + pos();
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

    if (xmlStream.attributes().hasAttribute("v-max"))
        vMax_ = xmlStream.attributes().value("v-max").toDouble();
    else
        vMax_ = MAXIMUM_VELOCITY;

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
    if (vMax_ < MAXIMUM_VELOCITY)
        xmlStream.writeAttribute("v-max", QString::number(vMax_));
    path_.save(xmlStream);
}
