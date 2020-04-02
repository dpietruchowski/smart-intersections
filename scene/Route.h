#ifndef ROUTE_H
#define ROUTE_H

#include <list>
#include <QXmlStreamReader>
#include <QXmlStreamWriter>

class PathItem;
class CarItem;
class IntersectionScene;
class CollisionPath;

class Route
{
public:
    using Iterator = std::list<PathItem*>::iterator;
    using ConstIterator = std::list<PathItem*>::const_iterator;
    Route(int id, IntersectionScene& intersection);

    Iterator begin();
    ConstIterator begin() const;
    Iterator end();
    ConstIterator end() const;

    bool load(QXmlStreamReader& xmlStream);
    void save(QXmlStreamWriter& xmlStream) const;

    int getId() const;

    const PathItem* getNextCommonPath(qreal distance, const Route* route) const;
    qreal getIntersectionLength(qreal distance, const Route* route) const;
    qreal getLengthAt(const PathItem* path, qreal pathLength) const;
    qreal getLength() const;
    std::pair<PathItem*, qreal> getPathAtDistance(qreal distance) const;
    PathItem* getNextPath(qreal currentDistance) const;
    CarItem* getNextCar(qreal distance) const;
    CollisionPath getNextCollisionPath(qreal distance);

    std::vector<CollisionPath> getCollisionPaths() const;
private:
    int id_;
    IntersectionScene& intersection_;
    std::list<PathItem*> paths_;
};

#endif // ROUTE_H
