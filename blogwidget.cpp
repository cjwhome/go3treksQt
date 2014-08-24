#include "blogwidget.h"
#include "ui_blogwidget.h"

BlogWidget::BlogWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::BlogWidget)
{
    ui->setupUi(this);
}

BlogWidget::~BlogWidget()
{
    delete ui;
}
