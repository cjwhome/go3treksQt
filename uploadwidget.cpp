#include "uploadwidget.h"
#include "ui_uploadwidget.h"

UploadWidget::UploadWidget(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::UploadWidget)
{
	ui->setupUi(this);
}

UploadWidget::~UploadWidget()
{
	delete ui;
}
