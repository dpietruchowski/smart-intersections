#include "CollisionPath.h"

#include "CarItem.h"

CollisionPath::CollisionPath(qreal in, qreal out, CollisionAreaItem* area)
    : distanceIn_(in), distanceOut_(out), area_(area)
{

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
