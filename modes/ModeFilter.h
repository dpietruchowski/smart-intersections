#ifndef MODEFILTER_H
#define MODEFILTER_H

#include <QObject>
#include <QGraphicsScene>
#include <QGraphicsSceneContextMenuEvent>
#include <QGraphicsSceneMouseEvent>
#include <QKeyEvent>

class ModeFilter: public QObject
{
public:
    enum Mode {
        ModeNone,
        ModeEditIntersection
    };

    ModeFilter(Mode mode);
    virtual ~ModeFilter() = default;

    Mode getMode() const;


protected:
    bool eventFilter(QObject *watched, QEvent *event);

    virtual void contextMenuEvent(QGraphicsSceneContextMenuEvent *) {}
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *) {}
    virtual void keyPressEvent(QKeyEvent *) {}

protected:
    Mode mode_;
    QGraphicsScene* watchedScene_ = nullptr;;
};

#endif // MODEFILTER_H
