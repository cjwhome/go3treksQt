#include "pomserialportwidget.h"
#include "ui_pomserialportwidget.h"
#include <QString>
#include <QObject>
#include <QFile>
#include <QDir>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QUrl>
#include <QUrlQuery>
#include <QNetworkReply>
#include <QJsonDocument>

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
                //ui->textBrowser->append("Found port:"+QString(serialPortInfoList[i].portName()));
				this->log("texttext");
                return 1;
                //qDebug()<<"Found Pom serial port";//<< POMserialPort->portName(); //Microchip Technology, Inc.
            }

    }
    //XXui->textBrowser->append("Could not find port");
    return 0;

}

void pomSerialPortWidget::pomConnect()
{
    if(POMserialPort.open(QIODevice::ReadWrite))
    {
        //XXui->textBrowser->append("Port Opened Successfully");
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
            //XXui->textBrowser->append("Creating Folder");
        }else{
            //XXui->textBrowser->append("Found Data Folder");
        }
        //XXif(QDir::setCurrent(combinedPath))
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
            return;
        }
        //XXui->textBrowser->append("Downloading Logged Data");
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
            //XXui->textBrowser->append("Created new name for file");
            pomfile.open(QIODevice::ReadWrite);
        }
    }
    POMserialPort.flush();
    POMserialPort.clear();
    if(QString::compare(QString(dataLine),"End logged data")>0){
        //XXui->textBrowser->append("Finished Downloading");
        transmittingData = false;
        POMserialPort.close();
        pomfile.close();
    }else if(madeNewFileName){

        pomfile.write(dataLine);
        //ui->textEdit->insertPlainText(dataLine);
    }

}


void pomSerialPortWidget::on_loginButton_clicked()
{
	
	// TODO: Display an error in these cases
	if (ui->usernameBox->text().isEmpty()) return;
	if (ui->passwordBox->text().isEmpty()) return;
	
	//XXui->textBrowser->append("Attempting login (takes a while)...");
	
	QNetworkAccessManager *nwam = new QNetworkAccessManager;
	
	QNetworkRequest r (QUrl("http://go3project.com/scripts/user/SE_CheckLogin.php"));
	r.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
	
	QString requestString("{\"Username\":\"" + ui->usernameBox->text() + "\",\"Email\":\"" + ui->passwordBox->text() + "\"}");
	
	// Build the POST data to be fed into the request
	QByteArray postData;
	QUrlQuery q;
    q.addQueryItem("Request", requestString);
    postData = q.query(QUrl::FullyEncoded).toUtf8();
	
	QNetworkReply *reply = nwam->post(r, postData);  // Actually send the request
	//while ( reply->waitForReadyRead(1));  // Block for up to 30s or until we've gotten our response
	
	QEventLoop loop;
	QTimer::singleShot(10000, &loop, SLOT(quit()));
	loop.exec();
	
	if (reply->error() != QNetworkReply::NoError) {
		//XXui->textBrowser->append("Request failed!  Error: "+reply->errorString());
		return;
	}
	
	//XXui->textBrowser->append("Returned "+QString(reply->readAll()));
	
	
	QJsonDocument response = QJsonDocument::fromJson(postData);
	
	
	
	//this->setCursor(QCursor(Qt::ArrowCursor));
	
	//XXui->textBrowser->append("Login successful!");
}






