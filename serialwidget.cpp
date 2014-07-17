#include "serialwidget.h"
#include "ui_serialwidget.h"

SerialWidget::SerialWidget(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::SerialWidget)
{
	ui->setupUi(this);
}

SerialWidget::~SerialWidget()
{
	delete ui;
}



void SerialWidget::connectToDevice() {
	
	//SerialStep* serialStep = new SerialStep(this);
	
	//connect(pomdevice, pomSerialPortWidget::log, logger, Logger::log);
	
	//if(pomdevice->findPomPort())
	//	pomdevice->pomConnect();
	//qDebug()<<"Found Port";
}
