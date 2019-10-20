#ifndef CARDISTANCESTAT_H
#define CARDISTANCESTAT_H

#include "Stat.h"

class CarDistanceStat: public Stat
{
public:
    CarDistanceStat();

    void visit(const CarItem& car, int currentTime) override;
};

#endif // CARDISTANCESTAT_H
