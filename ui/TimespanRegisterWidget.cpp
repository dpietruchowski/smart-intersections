#include "TimespanRegisterWidget.h"
#include "ui_TimespanRegisterWidget.h"

#include <QDebug>

class TableIntItem: public QTableWidgetItem
{

public:
    using QTableWidgetItem::QTableWidgetItem;
    bool operator< (const QTableWidgetItem &other) const
    {
        return (this->text().toInt() < other.text().toInt());
    }
};

TimespanRegisterWidget::TimespanRegisterWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TimespanRegisterWidget)
{
    ui->setupUi(this);

    connect(ui->stackedWidget, &QStackedWidget::currentChanged,
            [this] {
        int current = ui->stackedWidget->currentIndex();
        if (current < 0)
            return;
        QString format = "Area id: %1";
        qDebug() << current << idxMap_[current];
        ui->label->setText(format.arg(idxMap_[current]));
    });

    connect(ui->toolButtonPrev, &QToolButton::clicked, [this] {
        ui->stackedWidget->setCurrentIndex(ui->stackedWidget->currentIndex() - 1);
    });
    connect(ui->toolButtonNext, &QToolButton::clicked, [this] {
        ui->stackedWidget->setCurrentIndex(ui->stackedWidget->currentIndex() + 1);
    });
}

TimespanRegisterWidget::~TimespanRegisterWidget()
{
    delete ui;
}

void TimespanRegisterWidget::setCurrentArea(int id)
{
    int currIdx = 0;
    for(auto& idx: idxMap_) {
        if (idx.second == id) {
            currIdx = idx.first;
            break;
        }
    }
    ui->stackedWidget->setCurrentIndex(currIdx);
}

void TimespanRegisterWidget::addCollisionArea(int id)
{
    QWidget* page = new QWidget(ui->stackedWidget);
    QLayout* layout = new QVBoxLayout(page);
    QTableWidget* tableWidget = new QTableWidget(page);
    layout->addWidget(tableWidget);
    int idx = ui->stackedWidget->addWidget(page);
    idxMap_[idx] = id;
    tables_[id] = tableWidget;
    auto insertColumn = [tableWidget] (int idx, const QString& text) {
        tableWidget->insertColumn(idx);
        QTableWidgetItem *item = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(idx, item);
        item->setText(text);
    };
    insertColumn(Column::Time, "Time");
    insertColumn(Column::Timespan, "Timespan");
    insertColumn(Column::CarId, "Car id");
    ui->stackedWidget->setCurrentIndex(idx);
}

void TimespanRegisterWidget::addTime(int areaId, int carId, int time, int timespan)
{
    if(tables_.count(areaId) == 0)
        return;

    QTableWidget* tableWidget = tables_[areaId];
    int row = getTableItemRow(tableWidget, time);
    if (row != -1)
        return;

    tableWidget->insertRow(0);

    QTableWidgetItem* item = new TableIntItem(QString::number(time));
    tableWidget->setItem(0, Column::Time, item);
    item = new TableIntItem(QString::number(timespan));
    tableWidget->setItem(0, Column::Timespan, item);
    item = new TableIntItem(QString::number(carId));
    tableWidget->setItem(0, Column::CarId, item);
    tableWidget->sortByColumn(0, Qt::AscendingOrder);

    tableWidget->repaint();
}

void TimespanRegisterWidget::removeTime(int areaId, int carId, int time, int timespan)
{
    if(tables_.count(areaId) == 0)
        return;

    QTableWidget* tableWidget = tables_[areaId];
    int row = getTableItemRow(tableWidget, time);
    if (row == -1)
        return;

    tableWidget->removeRow(row);
    tableWidget->sortByColumn(0, Qt::AscendingOrder);

    tableWidget->repaint();
}

void TimespanRegisterWidget::clear()
{
    for(int i = (ui->stackedWidget->count() - 1); i >= 0; i--)
    {
        QWidget* widget = ui->stackedWidget->widget(i);
        ui->stackedWidget->removeWidget(widget);
        widget->deleteLater();
    }
    tables_.clear();
    idxMap_.clear();
}

int TimespanRegisterWidget::getTableItemRow(QTableWidget* table, int time)
{
    for (int row = 0; row < table->rowCount(); ++row) {
        QTableWidgetItem* item = table->item(row, Column::Time);
        if (!item) {
            qWarning("Item doesn't exist");
            continue;
        }
        int itemTime = item->text().toInt();
        if (time == itemTime)
            return row;
    }
    return -1;
}
