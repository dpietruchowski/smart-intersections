#include "IntersectionWidget.h"
#include "ui_IntersectionWidget.h"

#include <set>
#include <QFile>
#include <QStackedLayout>
#include <QDebug>
#include <QLabel>
#include "TimespanRegisterWidget.h"
#include "MainWindow.h"
#include "CollisionAreaItem.h"

#include <QGraphicsItem>

class TextEditView: public QObject
{
public:
    using QObject::QObject;

    void addTextEdit(QTextEdit* textEdit) {
        auto res = textEdits_.insert(textEdit);
        if (res.second)
            connect(textEdit, &QTextEdit::textChanged,
                    [this, textEdit] { setText(textEdit); });
    }

    void setAdditionalCallback(const std::function<void(QTextEdit*)>& cb) {
        additionalCallback_ = cb;
    }

private:
    void setText(QTextEdit* currEdit) {
        for (QTextEdit* edit: textEdits_) {
            if (edit == currEdit)
                continue;
            edit->blockSignals(true);
            edit->setText(currEdit->toPlainText());
            edit->blockSignals(false);
        }
        if (additionalCallback_)
            additionalCallback_(currEdit);
    }


private:
    std::set<QTextEdit*> textEdits_;
    std::function<void(QTextEdit*)> additionalCallback_;
};

IntersectionWidget::IntersectionWidget(const QString& name, QWidget *parent) :
    QWidget(parent), ui(new Ui::IntersectionWidget)
{
    ui->setupUi(this);

    auto initGraphicsView = [this] (QGraphicsView* view) {
        view->setScene(&scene_);
        view->setMouseTracking(true);
        view->scale(1, -1);
    };

    initGraphicsView(ui->graphicsView);
    initGraphicsView(ui->graphicsView_2);

    TextEditView* editView = new TextEditView(this);

    editView->addTextEdit(ui->textEdit);
    editView->addTextEdit(ui->textEdit_2);
    editView->setAdditionalCallback([this] (QTextEdit*) {
        if (isSaved())
            setWindowTitle(windowTitle() + '*');
        saved_ = false;
    });

    connect(ui->actionLoadScene, &QAction::triggered, [this] {
        QXmlStreamReader r(ui->textEdit->toPlainText());
        scene_.load(r);
        scene_.reset();
    });
    ui->toolButton->setDefaultAction(ui->actionLoadScene);

    connect(&scene_, &IntersectionScene::stepped, [this] (qreal time) {
        QString timeFormat = "Time: %1";
        ui->timeLabel->setText(timeFormat.arg(time));
        for(const auto& agent: scene_.getAgents()) {
            ui->carTable->updateAgent(agent.get());
        }
        ui->frame->setCurrentTime(scene_.getCurrentTime());
        ui->frame->update();
    });
    connect(&scene_, &IntersectionScene::focusItemChanged,
            [this] (QGraphicsItem* newFocusItem) {
        CollisionAreaItem* newArea = dynamic_cast<CollisionAreaItem*>(newFocusItem);
        ui->frame->update();
        if (newArea)
            ui->registerWidget->setCurrentArea(newArea->getId());
    });

    connect(&scene_.getManager(), &IntersectionManager::newCollisionArea,
            ui->registerWidget, &TimespanRegisterWidget::addCollisionArea);
    connect(&scene_.getManager(), &IntersectionManager::timeRegistered,
            ui->registerWidget, &TimespanRegisterWidget::addTime);
    connect(&scene_.getManager(), &IntersectionManager::timeUnregistered,
            ui->registerWidget, &TimespanRegisterWidget::removeTime);
    connect(&scene_.getManager(), &IntersectionManager::cleared,
            ui->registerWidget, &TimespanRegisterWidget::clear);

    ui->frame->setManager(&scene_.getManager());


    open(":/default.xml");
    setWindowTitle(name);
}

IntersectionWidget::~IntersectionWidget()
{
    delete ui;
}

IntersectionScene& IntersectionWidget::getScene()
{
    return scene_;
}

void IntersectionWidget::setView(IntersectionWidget::View view)
{
    ui->actionLoadScene->trigger();
    switch (view) {
        case View::Scene: {
            ui->stackedWidget->setCurrentIndex(0);
            break;
        }
        case View::Edit: {
            /*QString str;
            QXmlStreamWriter w(&str);
            w.setAutoFormatting(true);
            scene_.save(w);
            ui->textEdit->setText(str);*/
            ui->stackedWidget->setCurrentIndex(1);
            break;
        }
        case View::SceneEdit:
            ui->stackedWidget->setCurrentIndex(2);
            break;
    }
}

IntersectionWidget::View IntersectionWidget::getView() const
{
    switch (ui->stackedWidget->currentIndex()) {
        case 0: return View::Scene;
        case 1: return View::Edit;
        case 2: return View::SceneEdit;
        default: return View::Scene;
    }
}

bool IntersectionWidget::open(const QString& filename)
{
    if (filename.isEmpty()) {
        qWarning("Empty filename. Cannot open");
        return false;
    }

    QFile file(filename);
    file.open(QIODevice::ReadOnly);
    ui->textEdit->setText(file.readAll());
    setView(getView());
    file.close();

    if (!filename.startsWith(':')) {
        fileinfo_.setFile(filename);
        saved_ = true;
        setWindowTitle(fileinfo_.fileName());
    }

    return true;
}

void IntersectionWidget::save(const QString& filename)
{
    if (filename.isEmpty()) {
        qWarning("Empty filename. Cannot save");
        return;
    }

    fileinfo_.setFile(filename);
    save();
}

void IntersectionWidget::save()
{
    QFile file(fileinfo_.filePath());
    file.open(QIODevice::WriteOnly);
    if (!file.isWritable()) {
        qWarning("File is not writable");
        file.close();
        return;
    }

    saved_ = true;
    setWindowTitle(fileinfo_.fileName());
    QXmlStreamWriter w(&file);
    w.setAutoFormatting(true);
    scene_.save(w);
    file.close();


    QFile fileraw(fileinfo_.filePath() + "_raw");
    fileraw.open(QIODevice::WriteOnly);
    fileraw.write(ui->textEdit->toPlainText().toUtf8());
    fileraw.close();
}

bool IntersectionWidget::isSaved() const
{
    if (!fileExists())
        return false;

    return saved_;
}

bool IntersectionWidget::fileExists() const
{
    return fileinfo_.isFile();
}
