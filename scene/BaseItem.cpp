#include "BaseItem.h"

BaseItem::BaseItem(int id): id_(id)
{

}

void BaseItem::step()
{
    onStep();
}

void BaseItem::onStep()
{

}
