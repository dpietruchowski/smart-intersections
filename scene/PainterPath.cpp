#include "PainterPath.h"

#include <QXmlStreamReader>
#include <QXmlStreamWriter>


PainterPath::PainterPath(const PainterPath& other):
    QPainterPath(other), commands_(other.commands_)
{
}

PainterPath& PainterPath::operator=(const PainterPath& other)
{
    QPainterPath::operator=(other);
    commands_ = other.commands_;
    return *this;
}

PainterPath::PainterPath(const PainterPath&& other):
    QPainterPath(std::move(other)), commands_(std::move(other.commands_))
{
}

PainterPath& PainterPath::operator=(const PainterPath&& other)
{
    QPainterPath::operator=(std::move(other));
    commands_ = std::move(other.commands_);
    return *this;
}

bool PainterPath::load(QXmlStreamReader& xmlStream)
{
    if (!xmlStream.isStartElement() ||
            xmlStream.name() != "path")
        return false;

    if (commands_.size() > 0)
        qWarning("You are loading non empty path");
    while (xmlStream.readNextStartElement()) {
        QString name = xmlStream.name().toString();
        Points points = loadPoints(xmlStream);
        if (name == "line-to") {
            if (points.size() != 1)
                qWarning("Line to - wrong number of points");
            if (points.size() > 0)
                lineTo(points[0]);
        } else if (name == "quad-to") {
            if (points.size() != 2)
                qWarning("Quad to - wrong number of points");
            if (points.size() > 1)
                quadTo(points[0], points[1]);
        } else {
            xmlStream.skipCurrentElement();
        }
    }

    if (xmlStream.hasError())
        qWarning(xmlStream.errorString().toStdString().c_str());

    return true;
}

void PainterPath::save(QXmlStreamWriter& xmlStream) const
{
    for(auto command: commands_) {
        QString commandElement;
        switch(command.first) {
            case Command::LineTo:
                commandElement = "line-to";
                break;
            case Command::QuadTo:
                commandElement = "quad-to";
                break;
        }

        xmlStream.writeStartElement(commandElement);
        for(auto point: command.second) {
            xmlStream.writeStartElement("point");
            xmlStream.writeAttribute("x", QString::number(point.x()));
            xmlStream.writeAttribute("y", QString::number(point.y()));
            xmlStream.writeEndElement();
        }
        xmlStream.writeEndElement();
    }
}

PainterPath::Points PainterPath::loadPoints(QXmlStreamReader& xmlStream)
{
    Points points = {};
    while (xmlStream.readNextStartElement()) {
        if (xmlStream.name() == "point") {
            qreal x = xmlStream.attributes().value("x").toDouble();
            qreal y = xmlStream.attributes().value("y").toDouble();
            points.push_back({x, y});
        }
        xmlStream.skipCurrentElement();
    }
    return points;
}

void PainterPath::lineTo(const QPointF& p)
{
    commands_.push_back(std::make_pair(Command::LineTo, Points{p}));
    QPainterPath::lineTo(p);
}

void PainterPath::quadTo(const QPointF& ctrlPt, const QPointF& endPt)
{
    commands_.push_back(std::make_pair(Command::QuadTo, Points{ctrlPt, endPt}));
    QPainterPath::quadTo(ctrlPt, endPt);
}

QPointF PainterPath::pointAtLength(qreal length) const
{
    return pointAtPercent(percentAtLength(length));
}

qreal PainterPath::angleAtLength(qreal length) const
{
    return angleAtPercent(percentAtLength(length));
}
