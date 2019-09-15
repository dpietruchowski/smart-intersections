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
    explicit IntersectionWidget(const QString& name, QWidget *parent = nullptr);
    ~IntersectionWidget();

    IntersectionScene& getScene();

    enum class View {
        Scene,
        Edit,
        SceneEdit
    };

    void setView(View view);

private:
    QString name_;
    Ui::IntersectionWidget *ui;
    IntersectionScene scene_;
};

#endif // INTERSECIONSUBWINDOW_H
