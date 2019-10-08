#include "StandardModel.h"
#include "executionTime.h"
#include <QDebug>

void IdMap::setId(int id, int index)
{
    idMap_[id] = index;
}

int IdMap::getIndex(int id)
{
    if (idMap_.count(id) == 0)
        return -1;
    return idMap_[id];
}

int IdMap::getId(int index)
{
    for(auto [id, idx]: idMap_) {
        if (idx == index)
            return  id;
    }
    return -1;
}

void IdMap::eraseId(int id)
{
    idMap_.erase(id);
}

void IdMap::eraseIndex(int index)
{
    idMap_.erase(getId(index));
}

int StandardModel::setItemId(int rowId, int colId, QStandardItem* item)
{
    int inserted = -1;
    int rowIdx = rowMap_.getIndex(rowId);
    if (rowIdx == -1) {
        rowIdx = rowCount();
        insertRow(rowIdx);
    }

    int colIdx = colMap_.getIndex(colId);
    if (colIdx == -1) {
        colIdx = columnCount();
        insertColumn(colIdx);
        inserted = colIdx;
    }

    QStandardItemModel::setItem(rowIdx, colIdx, item);
    rowMap_.setId(rowId, rowIdx);
    colMap_.setId(colId, colIdx);

    return inserted;
}

QStandardItem* StandardModel::itemId(int rowId, int colId)
{

    int rowIdx = rowMap_.getIndex(rowId);
    int colIdx = rowMap_.getIndex(colId);
    if (rowIdx == -1 || colIdx == -1)
        return nullptr;

    return QStandardItemModel::item(rowIdx, colIdx);
}

void StandardModel::clear()
{
    QStandardItemModel::clear();
    rowMap_.clear();
    colMap_.clear();
}
