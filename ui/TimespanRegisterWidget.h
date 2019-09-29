#ifndef TIMESPANREGISTERWIDGET_H
#define TIMESPANREGISTERWIDGET_H

#include <QWidget>
#include <QTableWidget>

namespace Ui {
class TimespanRegisterWidget;
}

class TimespanRegisterWidget : public QWidget
{
    Q_OBJECT

public:
    explicit TimespanRegisterWidget(QWidget *parent = nullptr);
    ~TimespanRegisterWidget();

    enum Column {
        Time = 0,
        Timespan,
        CarId
    };

    void setCurrentArea(int id);
    void addCollisionArea(int id);
    void addTime(int areaId, int carId, int time, int timespan);
    void removeTime(int areaId, int carId, int time, int timespan);
    void clear();

private:
    int getTableItemRow(QTableWidget* table, int time);

private:
    std::map<int, QTableWidget*> tables_;
    std::map<int, int> idxMap_;
    Ui::TimespanRegisterWidget *ui;
};

#endif // TIMESPANREGISTERWIDGET_H
