#include "IntersectionWidget.h"
#include "ui_IntersectionWidget.h"

#include "modes/EditIntersectionMode.h"

IntersectionWidget::IntersectionWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::IntersectionWidget)
{
    ui->setupUi(this);
    ui->graphicsView->setScene(&scene_);
}

IntersectionWidget::~IntersectionWidget()
{
    delete ui;
}

IntersectionScene& IntersectionWidget::getScene()
{
    return scene_;
}

ModeFilter::Mode IntersectionWidget::getMode() const
{
    return mode_->getMode();
}

ModeFilter* IntersectionWidget::setMode(ModeFilter::Mode mode)
{
    if (mode_)
        scene_.removeEventFilter(mode_.get());
    switch(mode) {
        case ModeFilter::ModeNone:
            mode_ = nullptr;
            break;
        case ModeFilter::ModeEditIntersection:
            mode_ = std::make_unique<EditIntersectionMode>();
            break;
    }
    if (mode_) {
        scene_.installEventFilter(mode_.get());
        return mode_.get();
    } else {
        return nullptr;
    }
}
