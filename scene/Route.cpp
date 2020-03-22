#include "Route.h"

#include "IntersectionScene.h"
#include "PathItem.h"
#include "CarItem.h"

Route::Route(int id, IntersectionScene& intersection)
    : id_(id), intersection_(intersection)
{

}

Route::Iterator Route::begin()
{
    return paths_.begin();
}

Route::ConstIterator Route::begin() const
{
    return paths_.begin();
}

Route::Iterator Route::end()
{
    return paths_.end();
}

Route::ConstIterator Route::end() const
{
    return paths_.end();
}

bool Route::load(QXmlStreamReader& xmlStream)
{
    if (!xmlStream.isStartElement() ||
            xmlStream.name() != "route")
        return false;

    paths_.clear();
    id_ = xmlStream.attributes().value("id").toInt();
    while (xmlStream.readNextStartElement()) {
        if (xmlStream.name() == "path-id") {
            QString id = xmlStream.readElementText();
            PathItem* path = intersection_.getPath(id.toInt());
            if (path)
                paths_.push_back(path);
        } else {
            xmlStream.skipCurrentElement();
        }
    }
    return true;
}

void Route::save(QXmlStreamWriter& xmlStream) const
{
    xmlStream.writeStartElement("route");
    xmlStream.writeAttribute("id", QString::number(getId()));
    for (PathItem* path: paths_) {
        xmlStream.writeTextElement("path-id", QString::number(path->getId()));
    }
    xmlStream.writeEndElement();
}

int Route::getId() const
{
    return id_;
}

qreal Route::getLength() const
{
    qreal length = 0;
    for (auto* path: paths_) {
        length += path->path().length();
    }
    return length;
}

std::pair<PathItem*, qreal> Route::getPathAtDistance(qreal distance) const
{
    PathItem* desiredPath = nullptr;
    for (auto* path: paths_) {
        qreal pathLength = path->path().length();
        if (distance < pathLength) {
            desiredPath = path;
            break;
        }
        distance -= pathLength;
    }
    return std::make_pair(desiredPath, distance);
}

PathItem* Route::getNextPath(qreal currentDistance) const
{
    auto path = getPathAtDistance(currentDistance);
    if (!path.first)
        return nullptr;

    auto nextPath = std::find(paths_.begin(), paths_.end(), path.first);
    if (nextPath == paths_.end())
        return nullptr;
    nextPath = std::next(nextPath);
    if (nextPath == paths_.end())
        return nullptr;

    return *nextPath;
}

CarItem* Route::getNextCar(qreal distance) const
{
    auto path = getPathAtDistance(distance);

    if (!path.first)
        return nullptr;

    qreal pathDistance = path.second;
    auto nextPath = std::find(paths_.begin(), paths_.end(), path.first);
    while (nextPath != paths_.end()) {
        CarItem* car = (*nextPath)->getNextCar(pathDistance);
        if (car)
            return car;
        nextPath = std::next(nextPath);
        pathDistance = 0;
    }

    return nullptr;
}

std::vector<CollisionPath> Route::getCollisionPaths() const
{
    std::vector<CollisionPath> collisionPaths;


    /*PathItem* currPath = nullptr;
    qreal distance = 0;
    do {
        auto path = getPathAtDistance(distance);
        currPath = path.first;
        if (currPath) {
            CollisionPath cPath = currPath->getNextCollisionPath(distance);
            if (cPath.isValid()) {

                collisionPaths.push_back(cPath);
                distance = cPath.getInDistance();
            }
        }
    } while(currPath != nullptr);*/


    qreal offsetDistance = 0;
    CollisionPath lastCPath;
    for (auto* path: paths_) {
        auto cPaths = path->getCollisionPaths();
        for (auto cPath: cPaths) {
            cPath.setInDistance(cPath.getInDistance() + offsetDistance);
            cPath.setOutDistance(cPath.getOutDistance() + offsetDistance);
            if (lastCPath.getArea() == cPath.getArea()
                    && (lastCPath.getOutDistance() - cPath.getInDistance() < 0.01)) {
                collisionPaths.back() = merge(lastCPath, cPath);
            } else {
                collisionPaths.push_back(cPath);
            }
            lastCPath = collisionPaths.back();
        }

        offsetDistance += path->path().length();
    }


    return collisionPaths;
}


