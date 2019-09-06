#include "MainWindow.h"
#include "ui_MainWindow.h"

#include <QGraphicsItem>
#include <QMdiSubWindow>

#include "scene/IntersectionScene.h"
#include "scene/CarPathItem.h"

#include "ui/IntersectionWidget.h"
#include "ui/IntersectionKit.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->actionNewIntersection, &QAction::triggered,
            [this] {
        auto* window = ui->mdiArea->addSubWindow(new IntersectionWidget(ui->mdiArea));
        window->show();
    });

    connect(ui->actionEditIntersection, &QAction::triggered, this, &MainWindow::SetEditIntersectionMode);

    connect(ui->mdiArea, &QMdiArea::subWindowActivated, [this] {
        if (!ui->actionEditIntersection->isChecked())
            return;
        SetEditIntersectionMode(ui->actionEditIntersection->isChecked());
    });
}

MainWindow::~MainWindow()
{
    delete ui;
}

ModeFilter* MainWindow::SetIntersectionMode(ModeFilter::Mode mode)
{
    QList<QMdiSubWindow*> list = ui->mdiArea->subWindowList(QMdiArea::ActivationHistoryOrder);
    QMdiSubWindow* prevWindow = nullptr;
    if (list.size() > 1) {
        auto riter = list.rbegin();
        ++riter;
        prevWindow = *riter;
    }
    if (prevWindow)
        static_cast<IntersectionWidget*>(prevWindow->widget())->setMode(ModeFilter::ModeNone);
    auto* window = ui->mdiArea->activeSubWindow();
    if (window)
        return static_cast<IntersectionWidget*>(window->widget())->setMode(mode);
    return nullptr;
}

void MainWindow::SetEditIntersectionMode(bool checked)
{

    if (checked) {
        kit_ = std::make_unique<IntersectionKit>(this);
        addDockWidget(Qt::LeftDockWidgetArea, kit_.get());
        auto* mode = SetIntersectionMode(ModeFilter::ModeEditIntersection);
        if (mode) {
            kit_->SetEditIntersectionMode(static_cast<EditIntersectionMode*>(mode));
        }
    } else {
        kit_ = nullptr;
        SetIntersectionMode(ModeFilter::ModeNone);
    }
}
