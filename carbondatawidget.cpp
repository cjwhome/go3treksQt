#include "carbondatawidget.h"
#include "ui_carbondatawidget.h"

CarbonDataWidget::CarbonDataWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CarbonDataWidget)
{
    ui->setupUi(this);
}

CarbonDataWidget::~CarbonDataWidget()
{
    delete ui;
}

