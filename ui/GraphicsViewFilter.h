#ifndef GRAPHICSZOOM_H
#define GRAPHICSZOOM_H

#include <QObject>
#include <QPointF>

class QGraphicsView;

class GraphicsViewFilter: public QObject
{
public:
    GraphicsViewFilter(QGraphicsView* view);

    bool zoom(float factor);

protected:
    bool eventFilter(QObject *watched, QEvent *event) override;

private:
    QGraphicsView* view_;
    Qt::KeyboardModifiers modifiers_ = Qt::ControlModifier;
    float zoomFactorBase_ = 1.0015;
    QPointF targetScenePos_;
    QPointF targetViewportPos_;
};

#endif // GRAPHICSZOOM_H
