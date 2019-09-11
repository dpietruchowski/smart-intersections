#include "MainWindow.h"
#include "ui_MainWindow.h"

#include <QGraphicsItem>
#include <QMdiSubWindow>

#include "scene/IntersectionScene.h"
#include "scene/PathItem.h"

#include "ui/IntersectionWidget.h"

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

    connect(ui->actionEditIntersection, &QAction::triggered,
            [this] (bool checked){
        auto* intersection = currentIntersectionWidget();
        if (intersection)
                intersection->setView(checked ?
                                          IntersectionWidget::View::Edit :
                                          IntersectionWidget::View::Scene);
    });
}

MainWindow::~MainWindow()
{
    delete ui;
}

IntersectionWidget* MainWindow::currentIntersectionWidget()
{
    auto* window = ui->mdiArea->currentSubWindow()->widget();
    if (!window)
        return nullptr;
    return static_cast<IntersectionWidget*>(ui->mdiArea->currentSubWindow()->widget());
}
