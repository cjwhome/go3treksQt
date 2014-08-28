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

void BlogWidget::on_uploadButton_clicked()
{
	if (ui->cityBox->text().isEmpty()) {
		log("You must enter a city!");
		return;
	}
	if (ui->nameBox->text().isEmpty()) {
		log("You must enter a nickname!");
		return;
	}
	if (ui->descBox->toPlainText().isEmpty()) {
		log("You must enter a description!");
		return;
	}
	
	
	
	emit blogWritten(trekInfo);
}
