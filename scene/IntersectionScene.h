#ifndef INTERSECTIONSCENE_H
#define INTERSECTIONSCENE_H

#include <map>
#include <QBasicTimer>
#include <QGraphicsScene>
#include <QXmlStreamReader>
#include <QXmlStreamWriter>

#include "Route.h"

class PainterPath;
class PathItem;

class IntersectionScene: public QGraphicsScene
{
public:
    IntersectionScene();

    void reset();
    void start(int msec = 10);
    void stop();

    PathItem* addCarPath(const PainterPath &path,
                            const QPen &pen = QPen(),
                            const QBrush &brush = QBrush());

    PathItem* getPath(int id);
    Route* getRoute(int id);

    Route* createRoute(int id);
    void deleteRoute(int id);

    bool load(QXmlStreamReader& xmlStream);
    void save(QXmlStreamWriter& xmlStream) const;

protected:
    void timerEvent(QTimerEvent *event) override;
    void drawBackground(QPainter *painter, const QRectF &rect) override;
    void onStep();

private:
    void step();
    int getNextId() const;

private:
    QBasicTimer timer_;
    std::map<int, Route> routes_;
    mutable int nextId_ = 0;
};

#endif // INTERSECTIONSCENE_H
