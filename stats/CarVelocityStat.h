#ifndef CarVelocitystat_H
#define CarVelocitystat_H

#include "ByStepStat.h"

class CarVelocityStat: public ByStepStat
{
public:
    CarVelocityStat();

    void visit(const CarItem& car, int currentTime) override;
    void clear() override;
    const char* getName() const override { return "Velocity"; }
};

#endif // CarVelocitystat_H
