#include "BaseItem.h"

BaseItem::BaseItem(int id): id_(id)
{

}

void BaseItem::step()
{
    onStep();
}

void BaseItem::setId(int id)
{
    id_ = id;
    if (text_)
        text_->setText(QString::number(id));
}

void BaseItem::onStep()
{

}
