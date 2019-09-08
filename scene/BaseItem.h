#ifndef BASEITEM_H
#define BASEITEM_H

#include <QXmlStreamReader>
#include <QXmlStreamWriter>

class BaseItem
{
public:
    BaseItem();
    virtual ~BaseItem() = default;

    void step();


    virtual bool load(QXmlStreamReader& xmlStream) = 0;
    virtual void save(QXmlStreamWriter& xmlStream) const = 0;

protected:
    virtual void onStep();
};

#endif // BASEITEM_H
