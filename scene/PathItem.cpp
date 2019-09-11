#include "PathItem.h"

#include "CarItem.h"

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
    unsetBeginNode();
    unsetEndNode();
}

CarItem* PathItem::addCar(int id, qreal velocity, qreal distance)
{
    CarItem* car = new CarItem(id, this);
    car->setVelocity(velocity);
    car->setDistance(distance);

    addCar(car);
    return car;
}

CarItem* PathItem::addCar(qreal velocity, qreal distance)
{
    return addCar(-1, velocity, distance);
}

void PathItem::addCar(CarItem* car)
{
    updateCar(car);
    cars_.push_back(car);
}

void PathItem::removeCar(CarItem* car)
{
    cars_.remove(car);
}

bool PathItem::load(QXmlStreamReader& xmlStream)
{
    if (!xmlStream.isStartElement() ||
            xmlStream.name() != "car-path")
        return false;

    int id = xmlStream.attributes().value("id").toInt();
    qreal x = xmlStream.attributes().value("x").toDouble();
    qreal y = xmlStream.attributes().value("y").toDouble();
    setId(id);
    setPos(x, y);
    while (xmlStream.readNextStartElement()) {
        qDebug() << xmlStream.name();
        if (xmlStream.name() == "path") {
            PainterPath path;
            path.load(xmlStream);
            setPath(path);
        } else if (xmlStream.name() == "cars") {
            loadCars(xmlStream);
        } else {
            xmlStream.skipCurrentElement();
        }
    }
    return true;
}

void PathItem::save(QXmlStreamWriter& xmlStream) const
{
    xmlStream.writeStartElement("car-path");
    QPointF point = pos();
    xmlStream.writeAttribute("id", QString::number(getId()));
    xmlStream.writeAttribute("x", QString::number(point.x()));
    xmlStream.writeAttribute("y", QString::number(point.y()));
    path_.save(xmlStream);
    xmlStream.writeStartElement("cars");
    for(CarItem* car: cars_) {
        car->save(xmlStream);
    }
    xmlStream.writeEndElement();
    xmlStream.writeEndElement();
}

void PathItem::ConnectTo(PathItem* nextPath)
{
    if (hasEndNode() && nextPath->hasBeginNode()) {
        qWarning("Can't connect to");
        return;
    }

    if (hasEndNode()) {
        nextPath->setBeginNode(endNode_);
    } else if (nextPath->hasBeginNode()) {
        setEndNode(nextPath->beginNode_);
    } else {
        auto node = std::make_shared<PathNode>();
        setEndNode(node);
        nextPath->setBeginNode(node);
    }
}

bool PathItem::hasBeginNode() const
{
    return beginNode_ != nullptr;
}

bool PathItem::hasEndNode() const
{
    return endNode_ != nullptr;
}

void PathItem::unsetBeginNode()
{
    if (hasBeginNode())
        beginNode_->UnregisterNextPath(getId(), this);
    beginNode_ = nullptr;
}

void PathItem::unsetEndNode()
{
    if (hasEndNode())
        beginNode_->UnregisterPrevPath(getId(), this);
    endNode_ = nullptr;
}


void PathItem::setBeginNode(std::shared_ptr<PathNode>& node)
{
    unsetBeginNode();

    beginNode_ = node;
    if (node != nullptr)
        node->RegisterNextPath(getId(), this);
}

void PathItem::setEndNode(std::shared_ptr<PathNode>& node)
{
    unsetEndNode();

    endNode_ = node;
    if (node != nullptr)
        node->RegisterPrevPath(getId(), this);
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
    for (auto* car : cars_) {
        updateCar(car);
        if (percentAtCar(car) >= 100) {
            removeCar(car);
            delete car;
        }
    }
}

void PathItem::clearCars()
{
    for(auto* car : cars_) {
        delete car;
    }
    cars_.clear();
}

bool PathItem::loadCars(QXmlStreamReader& xmlStream)
{
    clearCars();
    while (xmlStream.readNextStartElement()) {
        if (xmlStream.name() == "car") {
            auto* car = addCar();
            if (!car->load(xmlStream)) {
                delete car;
                continue;
            }
            updateCar(car);
        } else {
            xmlStream.skipCurrentElement();
        }
    }
    return true;
}

std::pair<QPointF, qreal> PathItem::pointAtCar(const CarItem* car)
{
    QPainterPath _path = path();
    qreal percent = _path.percentAtLength(car->getDistance());
    QPointF pos = _path.pointAtPercent(percent);
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
