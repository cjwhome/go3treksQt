#include "quitwidget.h"
#include "ui_quitwidget.h"

QuitWidget::QuitWidget(QWidget *parent, QString path) :
	QWidget(parent),
	ui(new Ui::QuitWidget)
{
	ui->setupUi(this);
}

QuitWidget::~QuitWidget()
{
	delete ui;
}
