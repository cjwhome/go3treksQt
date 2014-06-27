#include "pomserialportwidget.h"
#include "ui_pomserialportwidget.h"
#include <QString>
#include <QObject>
#include <QFile>
#include <QDir>

pomSerialPortWidget::pomSerialPortWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::pomSerialPortWidget)
{
    ui->setupUi(this);
}

pomSerialPortWidget::~pomSerialPortWidget()
{
    delete ui;
}

bool pomSerialPortWidget::findPomPort()
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
                ui->textBrowser->append("Found port:"+QString(serialPortInfoList[i].portName()));
                return 1;
                //qDebug()<<"Found Pom serial port";//<< POMserialPort->portName(); //Microchip Technology, Inc.
            }

    }
    ui->textBrowser->append("Could not find port");
    return 0;

}

void pomSerialPortWidget::pomConnect()
{
    if(POMserialPort.open(QIODevice::ReadWrite))
    {
        ui->textBrowser->append("Port Opened Successfully");
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
            ui->textBrowser->append("Creating Folder");
        }else{
            ui->textBrowser->append("Found Data Folder");
        }
        if(QDir::setCurrent(combinedPath))
            ui->textBrowser->append("Set Path");
        //create file with temp name and change the name
        pomfile.setFileName("tempname.txt");
        if(pomfile.open(QIODevice::ReadWrite)){
            ui->textBrowser->append("Created new file");
            //pomfile.rename("newname.txt");
            //pomfile.write("blah blah");
            //pomfile.close();
        }else{
            ui->textBrowser->append("Could not open file");
        }
        ui->textBrowser->append("Downloading Logged Data");
        POMserialPort.write("t");
        POMserialPort.flush();
        POMserialPort.clear();
        transmittingData = true;
        madeNewFileName = false;
        connect(&POMserialPort, SIGNAL(readyRead()), this, SLOT(readData()));

    }
}

void pomSerialPortWidget::readData()
{

    QByteArray dataLine = POMserialPort.readAll();
    if(!madeNewFileName)
    {
        fields = QString(dataLine).split(QRegExp(","));
        for(int a=0;a<fields.size();a++)
        {
            qDebug()<<"field["<<a<<"]="<<fields[a];
        }
        if(fields.size()>10){
            QString newname = QString(fields[10].remove(QChar('/'))) + ".txt";
            pomfile.rename(newname);
            madeNewFileName=true;
            ui->textBrowser->append("Created new name for file");
            pomfile.open(QIODevice::ReadWrite);
        }
    }
    POMserialPort.flush();
    POMserialPort.clear();
    if(QString::compare(QString(dataLine),"End logged data")>0){
        ui->textBrowser->append("Finished Downloading");
        transmittingData = false;
        POMserialPort.close();
        pomfile.close();
    }else if(madeNewFileName){

        pomfile.write(dataLine);
        //ui->textEdit->insertPlainText(dataLine);
    }

}


