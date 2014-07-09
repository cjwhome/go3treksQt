#ifndef POMSERIALPORTWIDGET_H
#define POMSERIALPORTWIDGET_H


#include <QWidget>
#include <QtSerialPort/QtSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QDebug>
#include <QNetworkReply>

namespace Ui {
class pomSerialPortWidget;
}

class pomSerialPortWidget : public QWidget
{
    Q_OBJECT

public:
    explicit pomSerialPortWidget(QWidget *parent = 0);
    ~pomSerialPortWidget();
    bool findPomPort();
    void pomConnect();
	void addToTextLog(QString text);


private slots:
    void readData();
    void on_loginButton_clicked();

private:
    Ui::pomSerialPortWidget *ui;
    QSerialPort POMserialPort;
    QFile pomfile;
    QList<QSerialPortInfo> serialPortInfoList;
    QStringList fields;
    bool transmittingData;
    bool madeNewFileName;
	
	bool loggedIn = false;
	QString userName;
	QString userID;
	QString userRealName;
	QString userPassword;


};

#endif // POMSERIALPORTWIDGET_H
