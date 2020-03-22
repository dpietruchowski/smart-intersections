#include "BaseItem.h"

#include <QGraphicsItem>
#include <QPen>
#include <QBrush>
#include <IntersectionScene.h>
#include <QDebug>

#include <cstdlib>

namespace
{

QColor randomColor()
{
    QColor color;
    color.setRed(std::rand()%256);
    color.setBlue(std::rand()%256);
    color.setGreen(std::rand()%256);
    return color;
}

}

BaseItem::BaseItem(int id): id_(id)
{

}

BaseItem::~BaseItem() {
}

void BaseItem::reset()
{
    onReset();
}

void BaseItem::step(int currTime)
{
    onStep(currTime);
}

bool BaseItem::load(QXmlStreamReader& xmlStream)
{
    if (!xmlStream.isStartElement() ||
            xmlStream.name() != getItemName())
        return false;

    int id = xmlStream.attributes().value("id").toInt();
    setId(id);
    QColor color("black");
    QAbstractGraphicsShapeItem* thisItem = getItem<QAbstractGraphicsShapeItem>();
    if (xmlStream.attributes().hasAttribute("color") && thisItem) {
        color = xmlStream.attributes().value("color").toString();
    } else {
        color = randomColor();
    }
    QPen p = thisItem->pen();
    p.setWidth(2);
    p.setColor(color);
    thisItem->setPen(p);
    pen_ = p;
    return loadItem(xmlStream);
}

void BaseItem::save(QXmlStreamWriter& xmlStream)
{
    xmlStream.writeStartElement(getItemName());

    xmlStream.writeAttribute("id", QString::number(getId()));
    QAbstractGraphicsShapeItem* thisItem = getItem<QAbstractGraphicsShapeItem>();
    if (thisItem) {
        QPen p = thisItem->pen();
        xmlStream.writeAttribute("color", p.color().name());
    }
    saveItem(xmlStream);

    xmlStream.writeEndElement();
}

void BaseItem::setId(int id)
{
    id_ = id;
    QGraphicsItem* thisItem = getItem<QGraphicsItem>();
    if (thisItem)
        thisItem->setToolTip(getItemName() + QString(" id: ")+ QString::number(id_));
    if (text_)
        text_->setText(QString::number(id));
}

IntersectionScene* BaseItem::getIntersection()
{
    return dynamic_cast<IntersectionScene*>(getItem<QGraphicsItem>()->scene());
}
