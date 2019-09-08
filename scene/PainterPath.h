#ifndef PAINTERPATH_H
#define PAINTERPATH_H

#include <vector>
#include <list>
#include <QPainterPath>

class QXmlStreamReader;
class QXmlStreamWriter;

class PainterPath: public QPainterPath
{
public:
    using QPainterPath::QPainterPath;
    PainterPath(const PainterPath &other);
    PainterPath &operator=(const PainterPath &other);
    PainterPath(const PainterPath &&other);
    PainterPath &operator=(const PainterPath &&other);

    enum class Command {
        LineTo,
        QuadTo
    };

    bool load(QXmlStreamReader& xmlStream);
    void save(QXmlStreamWriter& xmlStream) const;

    void lineTo(const QPointF &p);
    void quadTo(const QPointF &ctrlPt, const QPointF &endPt);
    inline void lineTo(qreal x, qreal y);
    inline void quadTo(qreal ctrlPtx, qreal ctrlPty, qreal endPtx, qreal endPty);

private:
    using Points = std::vector<QPointF>;
    Points loadPoints(QXmlStreamReader& xmlStream);

private:
    using Commands = std::list<std::pair<Command, Points>>;
    Commands commands_;
};

inline void PainterPath::lineTo(qreal x, qreal y)
{
    lineTo(QPointF(x, y));
}

inline void PainterPath::quadTo(qreal ctrlPtx, qreal ctrlPty, qreal endPtx, qreal endPty)
{
    quadTo(QPointF(ctrlPtx, ctrlPty), QPointF(endPtx, endPty));
}

#endif // PAINTERPATH_H
