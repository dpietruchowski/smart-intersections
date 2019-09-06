#ifndef INTERSECTIONSCENE_H
#define INTERSECTIONSCENE_H

#include <QBasicTimer>
#include <QGraphicsScene>

class CarPathItem;

class IntersectionScene: public QGraphicsScene
{
public:
    IntersectionScene();

    void start(int msec);
    void stop();

    CarPathItem* addCarPath(const QPainterPath &path,
                            const QPen &pen = QPen(),
                            const QBrush &brush = QBrush());

protected:
    void timerEvent(QTimerEvent *event) override;
    void drawBackground(QPainter *painter, const QRectF &rect) override;
    void onStep();

private:
    void step();

private:
    QBasicTimer timer_;
};

#endif // INTERSECTIONSCENE_H
