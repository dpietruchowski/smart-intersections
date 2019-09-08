#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <memory>
#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class IntersectionWidget;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    IntersectionWidget* currentIntersectionWidget();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
