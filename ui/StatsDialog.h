#ifndef StatWidget_H
#define StatWidget_H

#include <memory>
#include <QDialog>

namespace Ui {
    struct StatsWidget;
}

class IntersectionWidget;

class StatsDialog: public QDialog
{
public:
    StatsDialog(QWidget* parent = nullptr);

    void setCurrentIntersection(IntersectionWidget* intersection);
    void reset();

private:
    std::unique_ptr<Ui::StatsWidget> ui_;
    IntersectionWidget* intersection_ = nullptr;
};

#endif // StatWidget_H
