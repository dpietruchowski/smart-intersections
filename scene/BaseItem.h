#ifndef BASEITEM_H
#define BASEITEM_H

class BaseItem
{
public:
    BaseItem();
    virtual ~BaseItem() = default;

    void step();

protected:
    virtual void onStep();
};

#endif // BASEITEM_H
