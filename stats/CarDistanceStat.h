#ifndef CARDISTANCESTAT_H
#define CARDISTANCESTAT_H

#include "ByStepStat.h"

class CarDistanceStat: public ByStepStat
{
public:
    CarDistanceStat();

    void visit(const CarItem& car, int currentTime) override;
    void clear() override;
    const char* getName() const override { return "Distance"; }
};

#endif // CARDISTANCESTAT_H
