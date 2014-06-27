#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "pomserialPortWidget.h"
#include <QVBoxLayout>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    pomSerialPortWidget* pomdevice = new pomSerialPortWidget();

    setCentralWidget(pomdevice);

    if(pomdevice->findPomPort())
        pomdevice->pomConnect();
        //qDebug()<<"Found Port";
}

MainWindow::~MainWindow()
{
    delete ui;
}
