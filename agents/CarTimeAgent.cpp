#include "CarTimeAgent.h"

#include "CarItem.h"

#include <QDebug>
#include <cmath>
#include "reverse.h"

CarTimeAgent::CarTimeAgent(CarItem* car): CarAgent(car)
{
}

CarTimeAgent::~CarTimeAgent()
{
}

void CarTimeAgent::registerAt(size_t id, int time, int timespan, CollisionAreaItem* area)
{
    if (id >= timespansRegister.size()) {
        qWarning("Wrong id");
        return;
    }

    if (timespansRegister[id].path.getArea() != area) {
        qWarning("Collision area not match. Registered for other area.");
    }

    timespansRegister[id].time = time;
    timespansRegister[id].timespan = timespan;
}
