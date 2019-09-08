#include "CarPathItem.h"

#include "CarItem.h"

#include <QDebug>

CarPathItem::CarPathItem(QGraphicsItem* parent): QGraphicsPathItem(parent)
{
    PainterPath path;
    path.moveTo(0, 0);
    path.quadTo(10, 20, 20, 20);
    path.lineTo(10, 10);
    setPath(path);
}

CarItem* CarPathItem::addCar(qreal velocity, qreal distance)
{
    CarItem* car = new CarItem(this);
    car->setVelocity(velocity);
    car->setDistance(distance);
    updateCar(car);

    cars_.push_back(car);
    return car;
}

bool CarPathItem::load(QXmlStreamReader& xmlStream)
{
    if (!xmlStream.isStartElement() ||
            xmlStream.name() != "car-path")
        return false;

    qreal x = xmlStream.attributes().value("x").toDouble();
    qreal y = xmlStream.attributes().value("y").toDouble();
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

void CarPathItem::save(QXmlStreamWriter& xmlStream) const
{
    xmlStream.writeStartElement("car-path");
    QPointF point = pos();
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

PainterPath CarPathItem::path() const
{
    return path_;
}

void CarPathItem::setPath(const PainterPath& path)
{
    path_ = path;
    QGraphicsPathItem::setPath(path);
}

void CarPathItem::onStep()
{
    for (auto* car : cars_) {
        updateCar(car);
        if (percentAtCar(car) >= 100) {
            cars_.remove(car);
            delete car;
        }
    }
}

void CarPathItem::clearCars()
{
    for(auto* car : cars_) {
        delete car;
    }
    cars_.clear();
}

bool CarPathItem::loadCars(QXmlStreamReader& xmlStream)
{
    clearCars();
    while (xmlStream.readNextStartElement()) {
        if (xmlStream.name() == "car") {
            auto* car = addCar();
            if (!car->load(xmlStream)) {
                delete car;
                continue;
            }
        } else {
            xmlStream.skipCurrentElement();
        }
    }
    return true;
}

std::pair<QPointF, qreal> CarPathItem::pointAtCar(const CarItem* car)
{
    QPainterPath _path = path();
    qreal percent = _path.percentAtLength(car->getDistance());
    QPointF pos = _path.pointAtPercent(percent);
    qreal angle = _path.angleAtPercent(percent);
    return std::make_pair(pos, angle);
}

qreal CarPathItem::percentAtCar(const CarItem* car)
{
    return path().percentAtLength(car->getDistance());
}

void CarPathItem::updateCar(CarItem* car)
{
    auto positionAngle = pointAtCar(car);
    car->setPos(positionAngle.first);
    car->setRotation(270-positionAngle.second);
}
