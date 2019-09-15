#include "MainWindow.h"
#include "ui_MainWindow.h"

#include <QGraphicsItem>
#include <QMdiSubWindow>
#include <QFile>
#include <QFileDialog>
#include <QXmlStreamWriter>
#include <QXmlStreamReader>
#include <QDebug>

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
        auto* window = ui->mdiArea->addSubWindow(
                    new IntersectionWidget(QString("New Intersection %1").arg(getNewId()), ui->mdiArea));
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

    connect(ui->actionStart, &QAction::triggered, [this] {
        auto* intersection = currentIntersectionWidget();
        if (intersection)
            intersection->getScene().start();
    });
    connect(ui->actionPause, &QAction::triggered, [this] {
        auto* intersection = currentIntersectionWidget();
        if (intersection)
            intersection->getScene().stop();
    });

    connect(ui->actionSaveIntersection, &QAction::triggered, [this] {
        saveCurrentIntersection();
    });

    connect(ui->mdiArea, &QMdiArea::subWindowActivated, [this] (QMdiSubWindow* window) {
        if (window)
            onCurrentIntersectionChanged(static_cast<IntersectionWidget*>(window->widget()));
    });
}

MainWindow::~MainWindow()
{
    delete ui;
}

IntersectionWidget* MainWindow::currentIntersectionWidget()
{
    if (!ui->mdiArea->currentSubWindow())
        return nullptr;
    auto* window = ui->mdiArea->currentSubWindow()->widget();
    if (!window)
        return nullptr;
    return static_cast<IntersectionWidget*>(ui->mdiArea->currentSubWindow()->widget());
}

void MainWindow::saveCurrentIntersection()
{
    IntersectionWidget* currInterwidget = currentIntersectionWidget();
    if (!currInterwidget)
        return;

    QString filename = QFileDialog::getSaveFileName(this,
      "Save intersection", {}, "XML files (*.xml)");

    if (filename.isEmpty())
        return;

    QFile file(filename);
    file.open(QIODevice::WriteOnly);
    QXmlStreamWriter w(&file);
    w.setAutoFormatting(true);

    IntersectionScene& scene = currInterwidget->getScene();
    scene.save(w);
}

void MainWindow::loadCurrentIntersection()
{

}

bool MainWindow::eventFilter(QObject* watched, QEvent* event)
{
    switch (event->type()) {
        case QEvent::GraphicsSceneMouseMove:
            mouseSceneMoveEvent(static_cast<QGraphicsSceneMouseEvent *>(event));
            break;
        default:
            break;
    }
    return false;
}

void MainWindow::mouseSceneMoveEvent(QGraphicsSceneMouseEvent* mouseEvent)
{
    auto pos = mouseEvent->scenePos();
    QString text = QString("(%1, %2)").arg(pos.x()).arg(pos.y());
    ui->statusBar->showMessage(text);
}

int MainWindow::getNewId()
{
    return nIntersections_++;
}

void MainWindow::onCurrentIntersectionChanged(IntersectionWidget* currentIntersection)
{
    QList<QMdiSubWindow*> intersections = ui->mdiArea->subWindowList(QMdiArea::ActivationHistoryOrder);
    IntersectionWidget* prevIntersection = nullptr;
    if (intersections.size() > 1) {
        auto riter = intersections.rbegin();
        ++riter;prevIntersection = static_cast<IntersectionWidget*>((*riter)->widget());
    }

    if (prevIntersection)
        prevIntersection->getScene().removeEventFilter(this);

    if (currentIntersection)
        currentIntersection->getScene().installEventFilter(this);
}
