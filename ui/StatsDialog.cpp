#include "StatsDialog.h"

#include "IntersectionWidget.h"

#include "ui_StatsWidget.h"

StatsDialog::StatsDialog(QWidget* parent): QDialog(parent), ui_(std::make_unique<Ui::StatsWidget>())
{
    ui_->setupUi(this);

    connect(ui_->pushButton, &QPushButton::clicked, [this] {
        if (!intersection_)
            return;

        for(auto& stat: intersection_->getScene().getStats()) {
            stat->refresh();
        }
    });
}

void StatsDialog::setCurrentIntersection(IntersectionWidget* intersection)
{
    if (intersection == intersection_)
        return;

    reset();
    if (!intersection)
        return;

    intersection_ = intersection;

    for(auto& stat: intersection_->getScene().getStats()) {
        QWidget* page = new QWidget;
        QLayout* layout = new QHBoxLayout(page);
        QChartView* view = new QChartView(page);
        layout->addWidget(view);
        QTableView* tview = new QTableView(page);
        layout->addWidget(tview);
        tview->setModel(stat->getModel());
        view->setChart(stat->getChart());
        ui_->stackedWidget->addWidget(page);
        ui_->comboBox->addItem(stat->getName());
    }
}

void StatsDialog::reset()
{
    for(int i = ui_->stackedWidget->count(); i >= 0; i--) {
        QWidget* widget = ui_->stackedWidget->widget(i);
        ui_->stackedWidget->removeWidget(widget);
    }
    ui_->comboBox->clear();
}
