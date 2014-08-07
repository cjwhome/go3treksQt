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

        //check if folder exists and create one if not
        QString combinedPath = QDir(QDir::home()).filePath("GO3TreksData");
        QDir dir(combinedPath);
        if(!dir.exists()){
            dir.mkpath(".");
            log("Creating Folder.\n");
        }else{
            log("Found Data Folder.\n");

        }
        if(QDir::setCurrent(combinedPath))
            log("Set Path.");

        //create file with temp name and change the name
        pomfile.setFileName("tempname.txt");
        if(pomfile.open(QIODevice::ReadWrite)){
            log("Opened ozone file for writing\n");
        }else{
            log("Could not open ozone file for writing\n");
            return false;
        }

        POMserialPort.write("t");           //send transmit command to get data
        POMserialPort.flush();
        POMserialPort.clear();
        transmittingData = true;
        madeNewFileName = false;
        logNumber = 0;          //reset for
        connect(&POMserialPort, SIGNAL(readyRead()), this, SLOT(readData()));

        return true;
    }else
        return false;

}

void SerialWidget::readData()
{

    QByteArray dataLine = POMserialPort.readAll();
    fields = QString(dataLine).split(QRegExp(","));     //fields are separated by commas
    if(!madeNewFileName)
    {
        if(fields.size()>10){
            QString newname = QString(fields[10].remove(QChar('/'))) + ".txt";
            pomfile.rename(newname);
            madeNewFileName=true;
            log("Created new name for file.\n");
            pomfile.open(QIODevice::ReadWrite);
        }
    }else{
       if(fields.size()>10)
       {
           logNumber = QString(fields[0]).toInt();      //keep track of the last log number
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
        emit transmitSuccessful(&pomfile);

    }else if(madeNewFileName){

        pomfile.write(dataLine);
        ui->transmitDisplay->append(dataLine);
        //ui->textEdit->insertPlainText(dataLine);
    }

}


