
#include "pomserialWidget.h"
#include "ui_pomserialWidget.h"
#include <QString>
#include <QMessageBox>

pomSerialWidget::pomSerialWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::pomSerialWidget)
{
    ui->setupUi(this);

}

void pomSerialWidget::listPorts()
{

    serialPortInfoList = QSerialPortInfo::availablePorts();
    for (int i = 0; i < serialPortInfoList.size(); ++i) {
        qDebug() << "- " << i << ": " <<
            serialPortInfoList[i].portName().toUtf8().constData() << ", " <<
            serialPortInfoList[i].manufacturer().toUtf8().constData() << ", " <<
            serialPortInfoList[i].description().toUtf8().constData() << "\n";
            if(QString::compare(QString(serialPortInfoList[i].manufacturer()),"Microchip Technology, Inc.")==0)
            {
                POMserialPort.setPort(serialPortInfoList[i]);
                qDebug()<<"Found Pom serial port";//<< POMserialPort->portName(); //Microchip Technology, Inc.
            }

    }

}
