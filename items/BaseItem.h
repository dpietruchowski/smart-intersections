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

    void reset();
    void step();

    bool load(QXmlStreamReader& xmlStream);
    void save(QXmlStreamWriter& xmlStream);

    int getId() const { return id_; }
    void setId(int id);

    template<class GraphicsClass>
    GraphicsClass* getItem() {
        return dynamic_cast<GraphicsClass*>(this);
    }


protected:
    virtual void onReset() {}
    virtual void onStep();

private:
    virtual const char* getItemName() = 0;
    virtual bool loadItem(QXmlStreamReader& xmlStream) = 0;
    virtual void saveItem(QXmlStreamWriter& xmlStream) const = 0;

protected:
    QGraphicsSimpleTextItem* text_ = nullptr;

private:
    int id_ = -1;
};

#endif // BASEITEM_H
