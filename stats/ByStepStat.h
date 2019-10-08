#ifndef BYSTEPSTAT_H
#define BYSTEPSTAT_H

#include <map>
#include <QVariant>

#include "Stat.h"
#include "StandardModel.h"

class ByStepStat: public Stat
{
public:
    ByStepStat();

    void clear() override;
    void refresh() override;

    QChart* getChart() override { return chart_; }
    QAbstractItemModel* getModel() override { return &model_; }

protected:
    std::map<int, std::map<int, QVariant>> data_;

private:
    void updateModel(int currentTime);

private:
    StandardModel model_;
    QChart* chart_ = nullptr;
};

#endif // BYSTEPSTAT_H
