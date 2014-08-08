#ifndef SERIALWIDGET_H
#define SERIALWIDGET_H

#include <QWidget>
#include <QString>
#include <QObject>
#include <QFile>
#include <QDir>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrl>
#include <QUrlQuery>
#include <QJsonDocument>
#include <QtSerialPort/QtSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QDebug>
#include <QDateTime>


namespace Ui {
class SerialWidget;
}

class SerialWidget : public QWidget
{
	Q_OBJECT
	
public:
	explicit SerialWidget(QWidget *parent = 0);
	~SerialWidget();
    bool findPomPort();
    bool setPOMTime();
	
signals:
	void log(QString text);
    void transmitSuccessful(QFile *fp);
    void foundPortSuccessful(QString portName);

public slots:
    bool connectPOM();
    void readData();
	
private:
	Ui::SerialWidget *ui;
    QList<QSerialPortInfo> serialPortInfoList;
    QSerialPort POMserialPort;
    QFile pomfile;
    QStringList fields;
    bool transmittingData;
    bool madeNewFileName;
    int logNumber;
    void delay();
};

#endif // SERIALWIDGET_H
