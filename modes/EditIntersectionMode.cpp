#include "EditIntersectionMode.h"

#include "ui/IntersectionKit.h"

EditIntersectionMode::EditIntersectionMode(): ModeFilter(ModeEditIntersection)
{
}

void EditIntersectionMode::SetCommand(EditIntersectionMode::Command cmd)
{
    cmd_ = cmd;
}

void EditIntersectionMode::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
    if (event->button() == Qt::LeftButton) {
        AddPoint(event->scenePos());
        switch (cmd_) {
            case CreateCmd: NewPath(); break;
            case MoveToCmd: MoveTo(); break;
            case LineToCmd: LineTo(); break;
            case QuadToCmd: QuadTo(); break;
            default: break;
        }
    }
}

QGraphicsEllipseItem* EditIntersectionMode::AddPoint(const QPointF& pos)
{
    QGraphicsEllipseItem* item = watchedScene_->addEllipse(-2.5, -2.5, 5, 5, QPen(Qt::red));
    tempPoints_.emplace_back(item, [] (QGraphicsItem* item) {
        if(item->scene())
            item->scene()->removeItem(item);
        delete item;
    });
    item->setPos(pos);

    if (tempPoints_.size() > 3)
        tempPoints_.pop_front();

    return item;
}

void EditIntersectionMode::ResetPoints()
{
    tempPoints_.clear();
}

void EditIntersectionMode::MoveTo()
{
    if (tempPoints_.size() == 0) return;
    if (!path_) return;
    QPointF pos = tempPoints_.back()->pos();
    path_->setPos(tempPoints_.back()->pos());
    ResetPoints();
    AddPoint(pos);
}

void EditIntersectionMode::LineTo()
{
    if (tempPoints_.size() == 0) return;
    if (!path_) return;
    auto path = path_->path();
    QPointF pos = tempPoints_.back()->pos();
    path.lineTo(tempPoints_.back()->pos() - path_->pos());
    path_->setPath(path);
    ResetPoints();
    AddPoint(pos);
}

void EditIntersectionMode::QuadTo()
{
    if (tempPoints_.size() < 3) return;
    if (!path_) return;
    auto path = path_->path();
    tempPoints_.pop_front();
    QPointF pos = tempPoints_.back()->pos();
    path.quadTo(tempPoints_.front()->pos() - path_->pos(),
                tempPoints_.back()->pos() - path_->pos());
    path_->setPath(path);
    ResetPoints();
    AddPoint(pos);
}

void EditIntersectionMode::NewPath()
{
    if (!watchedScene_)
        return;
    if (path_)
        path_->setPen(QPen(Qt::blue));
    path_ = new CarPathItem;
    watchedScene_->addItem(path_);
    SetCommand(MoveToCmd);
    MoveTo();
}
