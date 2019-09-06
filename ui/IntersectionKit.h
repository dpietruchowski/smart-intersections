#ifndef INTERSECTIONKIT_H
#define INTERSECTIONKIT_H

#include <QDockWidget>
#include "modes/EditIntersectionMode.h"

namespace Ui {
class IntersectionKit;
}

class IntersectionKit : public QDockWidget
{
    Q_OBJECT

public:
    explicit IntersectionKit(QWidget *parent = nullptr);
    ~IntersectionKit();

    void SetEditIntersectionMode(EditIntersectionMode* mode);

signals:
    void CommandTriggered(EditIntersectionMode::Command cmd);

private:
    void EmitCommandTriggered(EditIntersectionMode::Command cmd);

private:
    Ui::IntersectionKit *ui;

    EditIntersectionMode* mode_;
    QMetaObject::Connection connection_;
};

#endif // INTERSECTIONKIT_H
