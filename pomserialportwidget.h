#ifndef POMSERIALPORTWIDGET_H
#define POMSERIALPORTWIDGET_H


#include <QWidget>
#include <QtSerialPort/QtSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QDebug>

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



private slots:
    void readData();

private:

    Ui::pomSerialPortWidget *ui;
    QSerialPort POMserialPort;
    QFile pomfile;
    QList<QSerialPortInfo> serialPortInfoList;
    QStringList fields;
    bool transmittingData;
    bool madeNewFileName;


};

#endif // POMSERIALPORTWIDGET_H
