#include "MainWindow.h"
#include "ui_MainWindow.h"

#include <QGraphicsItem>
#include <QMdiSubWindow>

#include "scene/IntersectionScene.h"
#include "scene/CarPathItem.h"

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
}

MainWindow::~MainWindow()
{
    delete ui;
}
