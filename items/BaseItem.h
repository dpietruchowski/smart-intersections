#ifndef BASEITEM_H
#define BASEITEM_H

#include <QXmlStreamReader>
#include <QXmlStreamWriter>
#include <QGraphicsSimpleTextItem>
#include <QPen>
#include <QBrush>

#include "Stat.h"

class IntersectionScene;

class BaseItem
{
public:
    BaseItem(int id);
    BaseItem() = default;
    virtual ~BaseItem();;

    void reset();
    void step(int currTime);

    bool load(QXmlStreamReader& xmlStream);
    void save(QXmlStreamWriter& xmlStream);

    int getId() const { return id_; }
    void setId(int id);

    template<class GraphicsClass>
    GraphicsClass* getItem() {
        return dynamic_cast<GraphicsClass*>(this);
    }

    IntersectionScene* getIntersection();

    virtual void accept(Stat& Stat, int currentTime) {}

protected:
    virtual void onReset() {}
    virtual void onStep(int /*currTime*/) {}

private:
    virtual const char* getItemName() = 0;
    virtual bool loadItem(QXmlStreamReader& xmlStream) = 0;
    virtual void saveItem(QXmlStreamWriter& xmlStream) const = 0;

protected:
    QGraphicsSimpleTextItem* text_ = nullptr;
    QPen pen_;
    QBrush brush_;

private:
    int id_ = -1;
};

#endif // BASEITEM_H
