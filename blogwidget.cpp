#include "blogwidget.h"
#include "ui_blogwidget.h"

BlogWidget::BlogWidget(QWidget *parent, QString path) :
    QWidget(parent),
    ui(new Ui::BlogWidget)
{
    ui->setupUi(this);
	dataPath = path;
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
	
	trekInfo.City = ui->cityBox->text();
	
	// Name ex:  "Boulder, CO: Past 2B Tech (2014/08/25)"
	trekInfo.Name = ui->nameBox->text().append(": ");
	trekInfo.Name += ui->nameBox->text();
	trekInfo.Name += QString(" (").append(trekInfo.StartTime.toString("yyyy/MM/dd")).append(")");
	
	trekInfo.Description = ui->descBox->toPlainText();
	trekInfo.Description += QString("\n\nLocal start time: ").append(trekInfo.StartTime.toString("h:mm AP"));
	trekInfo.Description += QString("\nLocal end time: ").append(trekInfo.EndTime.toString("h:mm AP"));
	
	emit blogWritten(trekInfo);
}

void BlogWidget::setTimes(QDateTime start, QDateTime end) {
	trekInfo.StartTime = start;
	trekInfo.EndTime = end;
}
