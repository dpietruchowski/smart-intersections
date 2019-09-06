#ifndef INTERSECIONSUBWINDOW_H
#define INTERSECIONSUBWINDOW_H

#include <memory>
#include <QWidget>
#include "scene/IntersectionScene.h"
#include "modes/ModeFilter.h"
#include "ui/IntersectionKit.h"

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
    ModeFilter::Mode getMode() const;

    ModeFilter* setMode(ModeFilter::Mode mode);

private:
    Ui::IntersectionWidget *ui;
    IntersectionScene scene_;
    std::unique_ptr<ModeFilter> mode_;
};

#endif // INTERSECIONSUBWINDOW_H
