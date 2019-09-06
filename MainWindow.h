#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <memory>
#include <QMainWindow>

#include "modes/ModeFilter.h"

namespace Ui {
class MainWindow;
}

class IntersectionKit;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    ModeFilter* SetIntersectionMode(ModeFilter::Mode);
    void SetEditIntersectionMode(bool checked);

private:
    Ui::MainWindow *ui;
    std::unique_ptr<IntersectionKit> kit_;
};

#endif // MAINWINDOW_H
