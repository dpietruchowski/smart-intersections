#include "MainWindow.h"
#include "ui_MainWindow.h"

#include <QGraphicsItem>
#include <QMdiSubWindow>
#include <QFile>
#include <QFileDialog>
#include <QXmlStreamWriter>
#include <QXmlStreamReader>
#include <QDebug>
#include <QSpinBox>
#include <QActionGroup>

#include "scene/IntersectionScene.h"

#include "ui/IntersectionWidget.h"
#include "ui/TimerWidget.h"

class ViewActionGroup : public QActionGroup
{
public:
    using QActionGroup::QActionGroup;

    void setAction(IntersectionWidget::View view, QAction* action) {
        if (actions_.count(view)) {
            removeAction(actions_[view]);
        }

        actions_[view] = action;
        addAction(action);
    }

    void setView(IntersectionWidget::View view) {
        if (!actions_.count(view))
            return;

        actions_.at(view)->setChecked(true);
    }

    IntersectionWidget::View getView() const {
        QAction* checked = checkedAction();
        if (!checked)
            return IntersectionWidget::View::Scene;

        for (auto[view, action] : actions_) {
            if (action == checked)
                return view;
        }

        return IntersectionWidget::View::Scene;
    }

    void uncheckAll() {
        for (auto[view, action] : actions_) {
            action->setChecked(false);
        }
    }

private:
    std::map<IntersectionWidget::View, QAction*> actions_;
};

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->mdiArea->setViewMode(QMdiArea::TabbedView);

    auto *spinBox = new QSpinBox(this);
    spinBox->setRange(10, 200);
    spinBox->setValue(20);
    ui->toolBar->addWidget(spinBox);
    timer_ = new TimerWidget(this);
    ui->toolBar->addWidget(timer_);

    connect(ui->actionNewIntersection, &QAction::triggered,
            [this] {
        auto* window = ui->mdiArea->addSubWindow(
                    new IntersectionWidget(QString("New Intersection %1").arg(getNewId()), ui->mdiArea));
        window->show();
    });

    connect(ui->actionSaveIntersection, &QAction::triggered, [this] {
        saveCurrentIntersection();
    });

    connect(ui->actionOpenIntersection, &QAction::triggered, [this] {
        openIntersection();
    });

    ViewActionGroup *viewActionGroup = new ViewActionGroup(this);

    viewActionGroup->setAction(IntersectionWidget::View::Edit, ui->actionEditView);
    viewActionGroup->setAction(IntersectionWidget::View::Scene, ui->actionSceneView);
    viewActionGroup->setAction(IntersectionWidget::View::SceneEdit, ui->actionSceneEditView);

    connect(viewActionGroup, &QActionGroup::triggered, [this, viewActionGroup] {
        auto* intersection = currentIntersectionWidget();
        if (!intersection) {
            viewActionGroup->uncheckAll();
            return;
        }

        intersection->setView(viewActionGroup->getView());
    });

    connect(ui->mdiArea, &QMdiArea::subWindowActivated, [this, viewActionGroup] {
        auto* intersection = currentIntersectionWidget();
        if (intersection) {
            viewActionGroup->setView(intersection->getView());
        } else {
            viewActionGroup->uncheckAll();
        }
    });

    connect(ui->actionStart, &QAction::triggered, [this, spinBox] {
        auto* intersection = currentIntersectionWidget();
        if (intersection) {
            timer_->start();
            intersection->getScene().start(spinBox->value());
        }
    });
    connect(ui->actionPause, &QAction::triggered, [this] {
        auto* intersection = currentIntersectionWidget();
        if (intersection) {
            timer_->pause();
            intersection->getScene().stop();
        }
    });
    connect(ui->actionStop, &QAction::triggered, [this] {
        auto* intersection = currentIntersectionWidget();
        if (intersection) {
            timer_->stop();
            intersection->getScene().stop();
            intersection->getScene().reset();
        }
    });
    connect(ui->actionStep, &QAction::triggered, [this] {
        auto* intersection = currentIntersectionWidget();
        if (intersection) {
            intersection->getScene().step();
        }
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

    if (currInterwidget->fileExists()) {
        currInterwidget->save();
        return;
    }

    QString filename = QFileDialog::getSaveFileName(this,
      "Save intersection", {}, "XML files (*.xml)");

    if (filename.isEmpty())
        return;

    currInterwidget->save(filename);
}

void MainWindow::openIntersection()
{
    QString filename = QFileDialog::getOpenFileName(this,
      "Open intersection", {}, "XML files (*.xml);;All files (*)");

    if (filename.isEmpty())
        return;

    auto* window = ui->mdiArea->addSubWindow(
                new IntersectionWidget(QString("New Intersection %1").arg(getNewId()), ui->mdiArea));

    bool opened = static_cast<IntersectionWidget*>(window->widget())->open(filename);

    if (!opened)
        delete window;
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
