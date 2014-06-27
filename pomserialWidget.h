#ifndef POMSERIALWIDGET_H
#define POMSERIALWIDGET_H

#include <QWidget>
#include <QtSerialPort/QtSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QDebug>

namespace Ui {
  class pomSerialWidget;
}


class pomSerialWidget : public QWidget
{
    Q_OBJECT
public:
    explicit pomSerialWidget(QWidget *parent = 0);
    void listPorts();
signals:

public slots:

private:
    Ui::pomSerialWidget *ui;
    QSerialPort POMserialPort;
    QList<QSerialPortInfo> serialPortInfoList;

};

#endif // POMSERIALWIDGET_H
