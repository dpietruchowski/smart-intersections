#include "IntersectionWidget.h"
#include "ui_IntersectionWidget.h"

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
