#include "Stat.h"

#include <set>
#include <QFile>

void Stat::save(const char* filename) const
{
    QFile file(filename);
    file.open(QIODevice::WriteOnly);

    std::set<int> ids;
    for (auto&[step, dataForStep] : data_) {
        for (auto&[id, value]: dataForStep) {
            ids.insert(id);
        }
    }

    QString format = ",%1";
    QString strData;
    strData = format.arg(-1); // cols
    for(int id: ids) {
        strData += format.arg(id);
    }
    strData += '\n';
    for (auto&[step, dataForStep] : data_) {
        strData += format.arg(step);
        for (auto&[id, value]: dataForStep) {
            strData += format.arg(value.toString());
        }
        strData += '\n';
    }

    file.write(strData.toStdString().c_str());
    file.close();
}

void Stat::clear()
{
    data_.clear();
}
