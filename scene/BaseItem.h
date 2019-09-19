#ifndef BASEITEM_H
#define BASEITEM_H

#include <QXmlStreamReader>
#include <QXmlStreamWriter>
#include <QGraphicsSimpleTextItem>

class BaseItem
{
public:
    BaseItem(int id);
    BaseItem() = default;
    virtual ~BaseItem() = default;

    void step();


    virtual bool load(QXmlStreamReader& xmlStream) = 0;
    virtual void save(QXmlStreamWriter& xmlStream) const = 0;

    int getId() const { return id_; }
    void setId(int id);


protected:
    virtual void onStep();

protected:
    QGraphicsSimpleTextItem* text_ = nullptr;

private:
    int id_ = -1;
};

#endif // BASEITEM_H
