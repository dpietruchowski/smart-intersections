#include "CarTable.h"

#include <QHBoxLayout>

#include "CarAgent.h"
#include "CarItem.h"

class TableIntItem: public QTableWidgetItem
{

public:
    using QTableWidgetItem::QTableWidgetItem;
    bool operator< (const QTableWidgetItem &other) const
    {
        return (this->text().toInt() < other.text().toInt());
    }
};


CarTable::CarTable(QWidget* parent): QWidget(parent)
{
    auto* layout = new QHBoxLayout(this);
    clear();
}

void CarTable::clear()
{
    if (tableWidget)
        tableWidget->deleteLater();

    tableWidget = new QTableWidget(this);
    layout()->addWidget(tableWidget);

    auto insertColumn = [this] (int idx, const QString& text) {
        tableWidget->insertColumn(idx);
        QTableWidgetItem *item = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(idx, item);
        item->setText(text);
    };
    insertColumn(Column::CarId, "Car id");
    insertColumn(Column::Distance, "Distance");
    insertColumn(Column::Velocity, "Velocity");
    insertColumn(Column::MaxVelocity, "Max velocity");
}

void CarTable::updateAgent(const CarAgent* agent)
{
    int row = getRow(agent);
    if (row == -1) {
        insertAgent(agent);
        return;
    }
    auto setColumn = [this, row] (int column, qreal value) {
        QTableWidgetItem* item = tableWidget->item(row, column);
        item->setText(QString::number(value));
    };
    auto setColumnInt = [this, row] (int column, int value) {
        QTableWidgetItem* item = tableWidget->item(row, column);
        item->setText(QString::number(value));
    };

    setColumnInt(Column::CarId, agent->getCar()->getId());
    setColumn(Column::Distance, agent->getCar()->getDistance(CarItem::Front));
    setColumn(Column::Velocity, agent->getCar()->getDesiredVelocity());
    setColumn(Column::MaxVelocity, agent->getCar()->getMaxVelocity());
}

void CarTable::insertAgent(const CarAgent* agent)
{
    tableWidget->insertRow(0);

    auto setColumn = [this] (int column, qreal value) {
        QTableWidgetItem* item = new QTableWidgetItem(QString::number(value));
        tableWidget->setItem(0, column, item);
    };
    auto setColumnInt = [this] (int column, int value) {
        QTableWidgetItem* item = new TableIntItem(QString::number(value));
        tableWidget->setItem(0, column, item);
    };
    setColumnInt(Column::CarId, agent->getCar()->getId());
    setColumn(Column::Distance, agent->getCar()->getDistance(CarItem::Front));
    setColumn(Column::Velocity, agent->getCar()->getDesiredVelocity());
    setColumn(Column::MaxVelocity, agent->getCar()->getMaxVelocity());

    tableWidget->sortByColumn(Column::CarId, Qt::AscendingOrder);
}

int CarTable::getRow(const CarAgent* agent)
{
    for (int row = 0; row < tableWidget->rowCount(); ++row) {
        if (tableWidget->item(row, Column::CarId)->text().toInt() == agent->getCar()->getId())
            return row;
    }
    return -1;
}
