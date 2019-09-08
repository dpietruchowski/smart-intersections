#include "IntersectionEditor.h"
#include "ui_IntersectionEditor.h"

IntersectionEditor::IntersectionEditor(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::IntersectionEditor)
{
    ui->setupUi(this);
}

IntersectionEditor::~IntersectionEditor()
{
    delete ui;
}
