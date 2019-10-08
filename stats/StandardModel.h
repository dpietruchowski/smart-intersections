#ifndef STANDARDMODEL_H
#define STANDARDMODEL_H

#include <map>
#include <QStandardItemModel>

class IdMap {
public:
    void setId(int id, int idx);
    int getIndex(int id);
    int getId(int index);

    void eraseId(int id);
    void eraseIndex(int index);

    void clear() { idMap_.clear(); }

private:
    std::map<int, int> idMap_;
};

class StandardModel: public QStandardItemModel
{
public:
    StandardModel() = default;

    int setItemId(int rowId, int colId, QStandardItem* item);
    QStandardItem* itemId(int rowId, int colId);

    void clear();

private:

private:
    IdMap rowMap_;
    IdMap colMap_;
};

#endif // STANDARDMODEL_H
