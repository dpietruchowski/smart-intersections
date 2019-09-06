#include "ModeFilter.h"

ModeFilter::ModeFilter(ModeFilter::Mode mode): mode_(mode)
{

}

ModeFilter::Mode ModeFilter::getMode() const
{
    return mode_;
}

bool ModeFilter::eventFilter(QObject* watched, QEvent* event)
{
    watchedScene_ = qobject_cast<QGraphicsScene*>(watched);
    if (!watchedScene_) return false;

    switch (event->type()) {
        case QEvent::GraphicsSceneContextMenu:
            contextMenuEvent(static_cast<QGraphicsSceneContextMenuEvent *>(event));
            return true;
        case QEvent::GraphicsSceneMousePress:
            mousePressEvent(static_cast<QGraphicsSceneMouseEvent *>(event));
            return true;
        case QEvent::KeyPress:
            keyPressEvent(static_cast<QKeyEvent *>(event));
            return true;
        default:
            break;
    }
    return false;
}
