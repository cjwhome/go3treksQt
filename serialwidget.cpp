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



bool SerialWidget::connectToDevice() {
	
	//SerialStep* serialStep = new SerialStep(this);

    //connect(pomdevice, SerialWidget::log, logger, Logger::log);
	
    if(this->findPomPort())
        log("Found POM COM port.");
    else{
        log("Unable to find POM port, make sure POM usb is plugged in.  Try to disconnect and reconnect if already plugged in.");
        return 0;
    }
    if(POMserialPort.open(QIODevice::ReadWrite))
    {

        POMserialPort.setBaudRate(19200,QSerialPort::AllDirections);
        POMserialPort.setDataBits(QSerialPort::Data8);
        POMserialPort.setStopBits(QSerialPort::OneStop);
        POMserialPort.setFlowControl(QSerialPort::NoFlowControl);
        POMserialPort.setParity(QSerialPort::NoParity);

        //check if folder exists and create one if not
        QString combinedPath = QDir(QDir::home()).filePath("POMData");
        QDir dir(combinedPath);
        if(!dir.exists()){
            dir.mkpath(".");
            log("Creating Folder");
        }else{
            log("Found Data Folder.");

        }
        if(QDir::setCurrent(combinedPath))
            log("Set Path.");
            //XXui->textBrowser->append("Set Path");
        //create file with temp name and change the name
        pomfile.setFileName("tempname.txt");
        if(pomfile.open(QIODevice::ReadWrite)){
            //XXui->textBrowser->append("Created new file");
            //pomfile.rename("newname.txt");
            //pomfile.write("blah blah");
            //pomfile.close();
        }else{
            //XXui->textBrowser->append("Could not open file");
            return 0;
        }
        //XXui->textBrowser->append("Downloading Logged Data");
        POMserialPort.write("t");           //send transmit command to get data
        POMserialPort.flush();
        POMserialPort.clear();
        transmittingData = true;
        madeNewFileName = false;
        connect(&POMserialPort, SIGNAL(readyRead()), this, SLOT(readData()));
        return 1;
    }else
        return 0;
	//	pomdevice->pomConnect();
	//qDebug()<<"Found Port";
}

bool SerialWidget::findPomPort()
{

    serialPortInfoList = QSerialPortInfo::availablePorts();
    for (int i = 0; i < serialPortInfoList.size(); ++i) {
        /*qDebug() << "- " << i << ": " <<
            serialPortInfoList[i].portName().toUtf8().constData() << ", " <<
            serialPortInfoList[i].manufacturer().toUtf8().constData() << ", " <<
            serialPortInfoList[i].description().toUtf8().constData() << "\n";*/
            if(QString::compare(QString(serialPortInfoList[i].manufacturer()),"Microchip Technology, Inc.")==0)
            {
                POMserialPort.setPort(serialPortInfoList[i]);
                //ui->textBrowser->append("Found port:"+QString(serialPortInfoList[i].portName()));
                this->log("texttext");
                return 1;
                //qDebug()<<"Found Pom serial port";//<< POMserialPort->portName(); //Microchip Technology, Inc.
            }

    }
    //XXui->textBrowser->append("Could not find port");
    return 0;

}

void SerialWidget::readData()
{

    QByteArray dataLine = POMserialPort.readAll();
    if(!madeNewFileName)
    {
        fields = QString(dataLine).split(QRegExp(","));
        //for(int a=0;a<fields.size();a++)
        //{
        //    qDebug()<<"field["<<a<<"]="<<fields[a];
        //}
        if(fields.size()>10){
            QString newname = QString(fields[10].remove(QChar('/'))) + ".txt";
            pomfile.rename(newname);
            madeNewFileName=true;
            //XXui->textBrowser->append("Created new name for file");
            log("Created new name for file.");
            pomfile.open(QIODevice::ReadWrite);
        }
    }
    POMserialPort.flush();
    POMserialPort.clear();
    if(QString::compare(QString(dataLine),"End logged data")>0){
        //XXui->textBrowser->append("Finished Downloading");
        log("Finished Downloading.");
        transmittingData = false;
        POMserialPort.close();
        pomfile.close();
    }else if(madeNewFileName){

        pomfile.write(dataLine);
        //ui->textEdit->insertPlainText(dataLine);
    }

}


