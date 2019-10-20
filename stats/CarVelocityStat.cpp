#include "CarVelocityStat.h"

#include "CarItem.h"

CarVelocityStat::CarVelocityStat()
{
}

void CarVelocityStat::visit(const CarItem& car, int currentTime)
{
    data_[currentTime][car.getId()] = car.getVelocity();
}
