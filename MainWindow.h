#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <memory>
#include <QMainWindow>
#include <QGraphicsSceneMouseEvent>

namespace Ui {
class MainWindow;
}

class IntersectionWidget;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

    IntersectionWidget* currentIntersectionWidget();

    void saveCurrentIntersection();
    void openIntersection();

protected:
    bool eventFilter(QObject *watched, QEvent *event) override;
    void mouseSceneMoveEvent(QGraphicsSceneMouseEvent* mouseEvent);

private:

    int getNewId();
    void onCurrentIntersectionChanged(IntersectionWidget* currentIntersection);

private:
    Ui::MainWindow *ui;
    int nIntersections_ = 0;
};

#endif // MAINWINDOW_H
