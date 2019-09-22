#ifndef COLLISIONPATH_H
#define COLLISIONPATH_H

#include <QtGlobal>

class CollisionAreaItem;
class CarItem;

class CollisionPath
{
public:
    CollisionPath() = default;
    CollisionPath(qreal in, qreal out, CollisionAreaItem* area_);

    qreal getInDistance() const { return distanceIn_; }
    qreal getOutDistance() const { return distanceOut_; }
    CollisionAreaItem* getArea() { return area_; }

    bool isValid() const { return area_ != nullptr; }

    bool isInside(qreal distance);
    bool isPartInside(qreal backDist, qreal fronDist);
    bool isWholeInside(qreal backDist, qreal fronDist);

    friend CollisionPath merge(const CollisionPath& left, const CollisionPath& right);

private:
    qreal distanceIn_ = 0;
    qreal distanceOut_ = 0;
    CollisionAreaItem* area_ = nullptr;
};

#endif // COLLISIONPATH_H
