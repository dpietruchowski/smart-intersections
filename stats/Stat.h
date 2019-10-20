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

    void save(const char* filename) const;
    void clear()
;
    virtual void visit(const CarItem&, int /*currentTime*/) {}
    virtual void visit(const PathItem&, int /*currentTime*/) {}

protected:
    std::map<int, std::map<int, QVariant>> data_;
};

#endif // stat_H
