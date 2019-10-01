#ifndef TimelineWidget_H
#define TimelineWidget_H

#include <QFrame>

class IntersectionManager;

class TimelineWidget: public QFrame
{
public:
    static const int MAXTIME = 500;
    using QFrame::QFrame;

    void setManager(IntersectionManager* manager);
    void setCurrentTime(int time);

protected:
    void paintEvent(QPaintEvent *) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;

private:
    IntersectionManager* manager_ = nullptr;
    int currTime_ = 0;
    int beginTime = 0;
    int xPressed_ = 0;
};

#endif // TimelineWidget_H
