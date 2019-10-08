#ifndef stat_H
#define stat_H

#include <QChart>
#include <QtCharts>

class CarItem;
class PathItem;

class Stat
{
public:
    Stat() = default;
    virtual ~Stat() = default;

    virtual QChart* getChart() { return nullptr; }
    virtual QAbstractItemModel* getModel() { return nullptr; }

    virtual void visit(const CarItem&, int /*currentTime*/) {}
    virtual void visit(const PathItem&, int /*currentTime*/) {}
    virtual void clear() = 0;
    virtual void refresh() = 0;
    virtual const char* getName() const = 0;
};

#endif // stat_H
