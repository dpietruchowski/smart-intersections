#ifndef INTERSECIONSUBWINDOW_H
#define INTERSECIONSUBWINDOW_H

#include <memory>
#include <QWidget>
#include <QFileInfo>
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
    View getView() const;

    bool open(const QString& filename);
    void save(const QString& filename);
    void save();
    bool isSaved() const;
    bool fileExists() const;

private:
    QFileInfo fileinfo_;
    Ui::IntersectionWidget *ui;
    IntersectionScene scene_;
    bool saved_ = false;
};

#endif // INTERSECIONSUBWINDOW_H
