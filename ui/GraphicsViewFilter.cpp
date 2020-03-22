#include "GraphicsViewFilter.h"

#include <QApplication>
#include <QGraphicsView>
#include <QWheelEvent>
#include <QMouseEvent>
#include <QtMath>

GraphicsViewFilter::GraphicsViewFilter(QGraphicsView* view): QObject(view), view_(view)
{
    view->setMouseTracking(true);
    view->viewport()->installEventFilter(this);
}

bool GraphicsViewFilter::zoom(float factor)
{
    view_->scale(factor, factor);
    view_->centerOn(targetScenePos_);
    QPointF deltaViewportPos = targetViewportPos_ - QPointF(view_->viewport()->width() / 2.0,
                                                            view_->viewport()->height() / 2.0);
    QPointF viewport_center = view_->mapFromScene(targetScenePos_) - deltaViewportPos;
    view_->centerOn(view_->mapToScene(viewport_center.toPoint()));
}

bool GraphicsViewFilter::eventFilter(QObject*, QEvent* event)
{
    if (event->type() == QEvent::MouseMove) {
      QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);
      QPointF delta = targetViewportPos_ - mouseEvent->pos();
      if (qAbs(delta.x()) > 5 || qAbs(delta.y()) > 5) {
        targetViewportPos_ = mouseEvent->pos();
        targetScenePos_ = view_->mapToScene(mouseEvent->pos());
      }
    } else if (event->type() == QEvent::Wheel) {
      QWheelEvent* wheelEvent = static_cast<QWheelEvent*>(event);
      if (QApplication::keyboardModifiers() == modifiers_) {
        if (wheelEvent->orientation() == Qt::Vertical) {
          double angle = wheelEvent->angleDelta().y();
          double factor = qPow(zoomFactorBase_, angle);
          zoom(factor);
          return true;
        }
      }
    }
    return false;
}
