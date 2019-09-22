#include "Route.h"

#include "IntersectionScene.h"
#include "PathItem.h"

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

std::vector<CollisionPath> Route::getCollisionPaths() const
{
    std::vector<CollisionPath> collisionPaths;
    qreal distance = 0;
    for (auto* path: paths_) {
        CollisionPath cPath = path->getNextCollisionPath(distance);
        if (!cPath.isValid())
            break;

        collisionPaths.push_back(cPath);
        distance = cPath.getInDistance();
    }
    return collisionPaths;
}


