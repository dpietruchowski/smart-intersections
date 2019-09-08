#ifndef INTERSECIONSUBWINDOW_H
#define INTERSECIONSUBWINDOW_H

#include <memory>
#include <QWidget>
#include "scene/IntersectionScene.h"

namespace Ui {
class IntersectionWidget;
}

class IntersectionWidget : public QWidget
{
    Q_OBJECT

public:
    explicit IntersectionWidget(QWidget *parent = nullptr);
    ~IntersectionWidget();

    IntersectionScene& getScene();

private:
    Ui::IntersectionWidget *ui;
    IntersectionScene scene_;
};

#endif // INTERSECIONSUBWINDOW_H
