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
	
signals:
	void log(QString text);
	
public slots:
    bool connectToDevice();
    void readData();
	
private:
	Ui::SerialWidget *ui;
    QList<QSerialPortInfo> serialPortInfoList;
    QSerialPort POMserialPort;
    QFile pomfile;
    QStringList fields;
    bool transmittingData;
    bool madeNewFileName;
};

#endif // SERIALWIDGET_H
