#include "ByStepStat.h"

ByStepStat::ByStepStat()
{
    chart_ = new QChart();
}

void ByStepStat::clear()
{
    model_.clear();
    chart_->removeAllSeries();
}

void ByStepStat::refresh()
{
    int time = 0;
    model_.clear();
    chart_->removeAllSeries();
    for (auto&[currentTime, velocites]: data_) {
        updateModel(currentTime);
        time = currentTime;
    }
    chart_->createDefaultAxes();
    if (chart_->axisX())
        chart_->axisX()->setRange(0, time);
}

void ByStepStat::updateModel(int currentTime)
{
    QStandardItem* item = new QStandardItem(QString::number(currentTime));
    model_.setItemId(currentTime, -1, item);

    for(auto [id, velocity] : data_[currentTime]) {
        QStandardItem* item = new QStandardItem(velocity.toString());
        int inserted = model_.setItemId(currentTime, id, item);
        if (inserted >= 0) {
            QLineSeries* series = new QLineSeries();
            QVXYModelMapper* mapper = new QVXYModelMapper(chart_);
            series->setName(QString::number(inserted));
            mapper->setSeries(series);
            mapper->setXColumn(0);
            mapper->setYColumn(inserted);
            mapper->setModel(&model_);
            chart_->addSeries(series);
        }
    }
}
