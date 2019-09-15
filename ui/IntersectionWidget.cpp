#include "IntersectionWidget.h"
#include "ui_IntersectionWidget.h"

#include <QFile>
#include <QStackedLayout>

IntersectionWidget::IntersectionWidget(const QString& name, QWidget *parent) :
    QWidget(parent), name_(name),
    ui(new Ui::IntersectionWidget)
{
    ui->setupUi(this);
    ui->graphicsView->setScene(&scene_);
    ui->graphicsView->setMouseTracking(true);

    QFile file(":/default.xml");
    file.open(QIODevice::ReadOnly);
    ui->textEdit->setText(file.readAll());
    file.close();

    setView(View::Scene);

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
    switch (view) {
        case View::Scene: {
            QXmlStreamReader r(ui->textEdit->toPlainText());
            scene_.load(r);
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
            break;
    }
}
