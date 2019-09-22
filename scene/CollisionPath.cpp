#include "CollisionPath.h"

#include "CarItem.h"

CollisionPath::CollisionPath(qreal in, qreal out, CollisionAreaItem* area)
    : distanceIn_(in), distanceOut_(out), area_(area)
{

}

void CollisionPath::setInDistance(qreal distance)
{
    distanceIn_ = distance;
}

void CollisionPath::setOutDistance(qreal distance)
{
    distanceOut_ = distance;
}

bool CollisionPath::isInside(qreal distance)
{
    if (distance >= distanceIn_ && distance <= distanceOut_)
        return true;

    return false;
}

bool CollisionPath::isPartInside(qreal backDist, qreal fronDist)
{
    return backDist <= distanceOut_ && fronDist >= distanceIn_;
}

bool CollisionPath::isWholeInside(qreal backDist, qreal fronDist)
{
    return isInside(backDist) && isInside(fronDist);
}

CollisionPath merge(const CollisionPath& left, const CollisionPath& right)
{
    if (left.area_ != right.area_)
        return {};

    return CollisionPath(left.distanceIn_, right.distanceOut_, left.area_);
}
