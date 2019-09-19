#ifndef TIMERWIDGET_H
#define TIMERWIDGET_H

#include <QWidget>
#include <QTime>
#include <QTimer>

namespace Ui {
class TimerWidget;
}

class TimerWidget : public QWidget
{
    Q_OBJECT

public:
    explicit TimerWidget(QWidget *parent = nullptr);
    ~TimerWidget();

    void start();
    void pause();
    void stop();
    void reset();

private:
    void updateDisplay();

private:
    Ui::TimerWidget *ui;
    QTimer timer_;
    QDateTime startTime_;
};

#endif // TIMERWIDGET_H
