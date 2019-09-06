#ifndef EDITINTERSECTIONMODE_H
#define EDITINTERSECTIONMODE_H

#include <memory>

#include "ModeFilter.h"
#include "scene/CarPathItem.h"

class EditIntersectionMode: public ModeFilter
{
public:
    EditIntersectionMode();

    enum Command {
        CreateCmd,
        SelectCmd,
        RemoveCmd,
        MoveToCmd,
        LineToCmd,
        QuadToCmd
    };

    void SetCommand(Command cmd);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *) override;

private:
    QGraphicsEllipseItem *AddPoint(const QPointF& pos);
    void ResetPoints();
    void MoveTo();
    void LineTo();
    void QuadTo();
    void NewPath();

private:
    Command cmd_ = CreateCmd;
    using PointPtr = std::unique_ptr<QGraphicsEllipseItem,
                                     std::function<void(QGraphicsItem*)>>;
    CarPathItem* path_ = nullptr;
    std::list<PointPtr> tempPoints_;

};

#endif // EDITINTERSECTIONMODE_H
