#ifndef TIMESPANWINDOWS_H
#define TIMESPANWINDOWS_H

#include <QFrame>

class IntersectionManager;

class TimespanWindows: public QFrame
{
public:
    static const int MAXTIME = 500;
    using QFrame::QFrame;

    void setManager(IntersectionManager* manager);
    void setCurrentTime(int time);

protected:
    void paintEvent(QPaintEvent *) override;

private:
    IntersectionManager* manager_ = nullptr;
    int currTime_ = 0;
    int beginTime = 0;
};

#endif // TIMESPANWINDOWS_H
