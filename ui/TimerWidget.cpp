#include "TimerWidget.h"
#include "ui_TimerWidget.h"

#include <QDateTime>

TimerWidget::TimerWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TimerWidget)
{
    ui->setupUi(this);

    connect(&timer_, &QTimer::timeout, this, &TimerWidget::updateDisplay);
    reset();
}

TimerWidget::~TimerWidget()
{
    delete ui;
}

void TimerWidget::start()
{
    timer_.start(10);
}

void TimerWidget::pause()
{
    timer_.stop();
}

void TimerWidget::stop()
{
    timer_.stop();
    reset();
}

void TimerWidget::reset()
{
    startTime_ = QDateTime::currentDateTime();
    updateDisplay();
}

void TimerWidget::updateDisplay()
{
    qint64 elapsed = startTime_.msecsTo(QDateTime::currentDateTime());
    int h = static_cast<int>(elapsed / 1000 / 60 / 60);
    int m = static_cast<int>(elapsed / 1000 / 60) - (h * 60);
    int s = static_cast<int>(elapsed / 1000) - (m * 60);
    int ms = (static_cast<int>(elapsed) - (s * 1000)) / 10;

    ui->lcdMinutes->display(m);
    ui->lcdSeconds->display(s);
    ui->lcdMiliseconds->display(ms);
}
