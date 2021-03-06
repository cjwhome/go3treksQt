#include "serialwidget.h"
#include "ui_serialwidget.h"

SerialWidget::SerialWidget(QWidget *parent, QString path) :
	QWidget(parent),
	ui(new Ui::SerialWidget)
{
	ui->setupUi(this);
	dataPath = path;
}

SerialWidget::~SerialWidget()
{
	delete ui;
}

bool SerialWidget::setPOMTime()
{
	
	if(POMserialPort.open(QIODevice::ReadWrite) && disconnect(&POMserialPort, SIGNAL(readyRead()), this, SLOT(readData())))
	{
		QDateTime currentTime = QDateTime::currentDateTime();
		
		log("Current Date and Time: "+ currentTime.toString("hh:mm:ss dd/MM/yy"));
		POMserialPort.write("m");           //send menu command to enter menu
		POMserialPort.flush();
		POMserialPort.clear();
		log("Sent m to POM \n");
		delay(4);
		POMserialPort.write("c");           //send menu command to set time
		POMserialPort.flush();
		POMserialPort.clear();
		log("Sent c to POM");
		delay(2);
		QString str = currentTime.toString("hhmmss");
		QByteArray ba = str.toLocal8Bit();
		const char *c_str = ba.data();
		//POMserialPort.write();
		POMserialPort.write(c_str);
		log("Sent new time to POM");
		delay(2);
		POMserialPort.write("d");           //send menu command to set date
		POMserialPort.flush();
		POMserialPort.clear();
		log("Sent d to POM");
		delay(2);
		str = currentTime.toString("ddMMyy");
		ba = str.toLocal8Bit();
		const char *d_str = ba.data();
		//POMserialPort.write();
		POMserialPort.write(d_str);
		log("Sent date to POM.");
		delay(2);
		log("Sent x to POM");
		POMserialPort.write("x");           //send menu command to exit menu
		delay(2);
		POMserialPort.flush();
		POMserialPort.clear();
		POMserialPort.close();
		
		connect(&POMserialPort, SIGNAL(readyRead()), this, SLOT(readData()));
		log("Set date and time to PC time\n");
	}else{
		log("Unable to open port to set time\n");
		return false;
	}
}

bool SerialWidget::findPomPort()
{
	serialPortInfoList = QSerialPortInfo::availablePorts();
	for (int i = 0; i < serialPortInfoList.size(); ++i) {
		
		if(QString::compare(QString(serialPortInfoList[i].manufacturer()),"Microchip Technology, Inc.")==0)
		{
			POMserialPort.setPort(serialPortInfoList[i]);
			emit foundPortSuccessful(QString(serialPortInfoList[i].portName()));
			return true;
		}
	}
	log("Could not find POM connection\n");
	return false;
}

bool SerialWidget::findPomPortAgain()
{
	serialPortInfoList = QSerialPortInfo::availablePorts();
	for (int i = 0; i < serialPortInfoList.size(); ++i) {
		
		if(QString::compare(QString(serialPortInfoList[i].manufacturer()),"Microchip Technology, Inc.")==0)
		{
			POMserialPort.setPort(serialPortInfoList[i]);
			return true;
		}
	}
	log("Could not find POM connection\n");
	return false;
}

bool SerialWidget::connectPOM() {
	
	
	/*if(this->findPomPort())
		log("Found POM COM port.\n");
	else{
		log("Unable to find POM port, make sure POM usb is plugged in.  Try to disconnect and reconnect if already plugged in.\n");
		return false;
	}*/
	
	if(POMserialPort.open(QIODevice::ReadWrite))        //open the port!
	{
		
		POMserialPort.setBaudRate(19200,QSerialPort::AllDirections);
		POMserialPort.setDataBits(QSerialPort::Data8);
		POMserialPort.setStopBits(QSerialPort::OneStop);
		POMserialPort.setFlowControl(QSerialPort::NoFlowControl);
		POMserialPort.setParity(QSerialPort::NoParity);
		
		//create file with temp name and change the name
		pomfile.setFileName("RawPom.txt");
		if(pomfile.open(QIODevice::ReadWrite | QIODevice::Truncate)) {
			log("Downloading instrument data...");
		} else {
			log("Could not open RawPom file for writing\n");
			return false;
		}
		
		POMserialPort.write("t");  // Send transmit command to get data
		POMserialPort.flush();
		POMserialPort.clear();
		transmittingData = true;
		madeNewFileName = false;
		logNumber = 0;          //reset for
		connect(&POMserialPort, SIGNAL(readyRead()), this, SLOT(readData()));
		
		return true;
	} else
		return false;

}

void SerialWidget::readData()
{
	QByteArray dataLine = POMserialPort.readAll();
	/*fields = QString(dataLine).split(QRegExp(","));     //fields are separated by commas
	if(!madeNewFileName)
	{
		if(fields.size()>10) {
			QString newname = QString(fields[10].remove(QChar('/'))) + ".txt";
			pomfile.close();
			pomfile.rename(newname);
			madeNewFileName=true;
			log("Created new name for file.\n");
			pomfile.open(QIODevice::ReadWrite);
		}
	} else {
		if(fields.size()>10)
		{
			logNumber = QString(fields[0]).toInt();      //keep track of the last log number
		}
	}*/
	
	POMserialPort.flush();
	POMserialPort.clear();
	if(QString::compare(QString(dataLine),"End logged data")>0) {
		log("POM data downloaded!");
		transmittingData = false;
		POMserialPort.close();
		pomfile.close();
		emit transmitSuccessful(&pomfile);
		
	}
	else {
		pomfile.write(dataLine);
		ui->transmitDisplay->append(dataLine);
	}
	
}

bool SerialWidget::restartLogging()
{
	if(findPomPortAgain())
	{
		if(POMserialPort.isOpen())
		{
			POMserialPort.write("l");  // Send log command to restart logging
			POMserialPort.flush();
			POMserialPort.clear();
			POMserialPort.close();
			return true;
		}else if(POMserialPort.open(QIODevice::ReadWrite))        //open the port!
		{
			POMserialPort.setBaudRate(19200,QSerialPort::AllDirections);
			POMserialPort.setDataBits(QSerialPort::Data8);
			POMserialPort.setStopBits(QSerialPort::OneStop);
			POMserialPort.setFlowControl(QSerialPort::NoFlowControl);
			POMserialPort.setParity(QSerialPort::NoParity);
			
			
			POMserialPort.write("l");  // Send log command to restart logging
			POMserialPort.flush();
			POMserialPort.clear();
			POMserialPort.close();
			return true;	
		}
		else
		{
			log("Could not open POM port to restart logger");
			return false;
		}
	}else{
		log("Could not find POM port to restart logger");
		return false;
	}
	
}

void SerialWidget::delay(int seconds)
{
	QTime dieTime= QTime::currentTime().addSecs(seconds);
	while( QTime::currentTime() < dieTime )
	QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
}


