#include "CarDistanceStat.h"

#include "CarItem.h"

CarDistanceStat::CarDistanceStat()
{

}

void CarDistanceStat::visit(const CarItem& car, int currentTime)
{
    data_[currentTime][car.getId()] = car.getRouteDistance();
}

void CarDistanceStat::clear()
{
    data_.clear();
}
