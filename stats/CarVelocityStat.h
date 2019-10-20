#ifndef CarVelocitystat_H
#define CarVelocitystat_H

#include "Stat.h"

class CarVelocityStat: public Stat
{
public:
    CarVelocityStat();

    void visit(const CarItem& car, int currentTime) override;
};

#endif // CarVelocitystat_H
