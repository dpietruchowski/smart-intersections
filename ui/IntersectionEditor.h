#ifndef INTERSECTIONEDITOR_H
#define INTERSECTIONEDITOR_H

#include <QWidget>

namespace Ui {
class IntersectionEditor;
}

class IntersectionEditor : public QWidget
{
    Q_OBJECT

public:
    explicit IntersectionEditor(QWidget *parent = nullptr);
    ~IntersectionEditor();

private:
    Ui::IntersectionEditor *ui;
};

#endif // INTERSECTIONEDITOR_H
