#include "IntersectionKit.h"
#include "ui_IntersectionKit.h"

#include <QAction>

IntersectionKit::IntersectionKit(QWidget *parent) :
    QDockWidget(parent),
    ui(new Ui::IntersectionKit)
{
    QWidget* widget = new QWidget(this);
    ui->setupUi(widget);
    setWidget(widget);

    setAllowedAreas(Qt::LeftDockWidgetArea);

    connect(ui->btnNewPath, &QToolButton::clicked,
            [this] { EmitCommandTriggered(EditIntersectionMode::CreateCmd); });
    connect(ui->btnSelectPath, &QToolButton::clicked,
            [this] { EmitCommandTriggered(EditIntersectionMode::SelectCmd); });
    connect(ui->btnDeletePath, &QToolButton::clicked,
            [this] { EmitCommandTriggered(EditIntersectionMode::RemoveCmd); });
    connect(ui->btnMoveTo, &QToolButton::clicked,
            [this] { EmitCommandTriggered(EditIntersectionMode::MoveToCmd); });
    connect(ui->btnLineTo, &QToolButton::clicked,
            [this] { EmitCommandTriggered(EditIntersectionMode::LineToCmd); });
    connect(ui->btnQuadTo, &QToolButton::clicked,
            [this] { EmitCommandTriggered(EditIntersectionMode::QuadToCmd); });
}

IntersectionKit::~IntersectionKit()
{
    delete ui;
}

void IntersectionKit::SetEditIntersectionMode(EditIntersectionMode* mode)
{
    if (connection_)
        disconnect(connection_);
    mode_ = mode;
    if (mode_)
        connection_ = connect(this, &IntersectionKit::CommandTriggered, mode_, &EditIntersectionMode::SetCommand);
}

void IntersectionKit::EmitCommandTriggered(EditIntersectionMode::Command cmd)
{
    emit CommandTriggered(cmd);
}
