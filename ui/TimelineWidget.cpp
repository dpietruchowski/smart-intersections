#include "TimelineWidget.h"

#include <QPainter>
#include <QMouseEvent>

#include "IntersectionTimeManager.h"
#include "IntersectionQueueManager.h"
#include "CarTimeAgent.h"
#include "CarItem.h"
#include "reverse.h"

void TimelineWidget::setManager(IntersectionManager* manager)
{
    manager_ = manager;
}

void TimelineWidget::setCurrentTime(int time)
{
    currTime_ = time;
    if (currTime_ < beginTime)
        beginTime = currTime_;
}

void TimelineWidget::paintEvent(QPaintEvent* event)
{
    QFrame::paintEvent(event);

    int w = width();
    int h = height();

    QPainter painter(this);
    painter.setBrush(QColor(100, 100, 200, 200));
    painter.drawRect(0, 0, w, h);

    paint(dynamic_cast<IntersectionTimeManager*>(manager_), painter);
    paint(dynamic_cast<IntersectionQueueManager*>(manager_), painter);
}

void TimelineWidget::mouseMoveEvent(QMouseEvent* event)
{
    int diff = event->pos().x() - xPressed_;
    int w = width();
    qreal j = (qreal)w / MAXTIME;
    beginTime = j * diff;
    update();
}

void TimelineWidget::mousePressEvent(QMouseEvent* event)
{
    xPressed_ = event->pos().x();
}

void TimelineWidget::mouseReleaseEvent(QMouseEvent* event)
{
    xPressed_ = 0;
}

void TimelineWidget::paint(IntersectionTimeManager* manager, QPainter& painter)
{
    if (!manager)
        return;

    int w = width();
    int h = height();
    qreal j = (qreal)w / MAXTIME;

    painter.save();
    painter.setPen(QColor(0,0,0));
    painter.setBrush(QColor(200, 200, 200, 200));
    auto* r = manager->getCurrentRegister();
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

void TimelineWidget::paint(IntersectionQueueManager* manager, QPainter& painter)
{
    if (!manager)
        return;

    int w = width();
    int h = height();

    painter.save();
    painter.setPen(QColor(0,0,0));
    painter.setBrush(QColor(200, 200, 200, 200));
    auto queue = manager->getCurrentQueue();
    qreal j = (qreal)w / queue.size();
    int i = 0;
    for (auto* agent: reverse(queue)) {
        int x = i*j;
        painter.drawRect(x, 0, j, h);
        painter.drawText(x + 1, h/2, QString::number(agent->getCar()->getId()));
        i++;
    }
    painter.restore();
}
