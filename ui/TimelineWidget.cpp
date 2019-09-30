#include "TimespanWindows.h"

#include <QPainter>
#include <QMouseEvent>

#include "IntersectionManager.h"
#include "CarAgent.h"
#include "CarItem.h"

void TimespanWindows::setManager(IntersectionManager* manager)
{
    manager_ = manager;
}

void TimespanWindows::setCurrentTime(int time)
{
    currTime_ = time;
    if (currTime_ < beginTime)
        beginTime = currTime_;
}

void TimespanWindows::paintEvent(QPaintEvent* event)
{
    QFrame::paintEvent(event);

    int w = width();
    int h = height();
    qreal j = (qreal)w / MAXTIME;

    QPainter painter(this);
    painter.setBrush(QColor(100, 100, 200, 200));
    painter.drawRect(0, 0, w, h);

    painter.save();
    painter.setPen(QColor(0,0,0));
    painter.setBrush(QColor(200, 200, 200, 200));
    auto* r = manager_->getCurrentRegister();
    if (r)
        for (auto& t : *r) {
            int x = j * (t.second.time - beginTime);
            int xd = j * t.second.timespan;
            painter.drawRect(x, 0, xd, h);
            painter.drawText(x + 1, h/2, QString::number(t.second.agent->getCar()->getId()));
        }
    painter.restore();
    int x = (currTime_ - beginTime) * j;
    if (currTime_ - beginTime > MAXTIME)
        beginTime = currTime_;
    painter.fillRect(x, 0, 2, h, QColor(0, 0, 0));
}

void TimespanWindows::mouseMoveEvent(QMouseEvent* event)
{
    int diff = event->pos().x() - xPressed_;
    int w = width();
    qreal j = (qreal)w / MAXTIME;
    beginTime = j * diff;
    update();
}

void TimespanWindows::mousePressEvent(QMouseEvent* event)
{
    xPressed_ = event->pos().x();
}

void TimespanWindows::mouseReleaseEvent(QMouseEvent* event)
{
    xPressed_ = 0;
}
