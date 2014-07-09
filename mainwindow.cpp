#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent)
{
	
	TEMPTBD = new QListWidget(this);
	
	/// Widget Stack ///
	mainWidgetStack = new QStackedWidget(this);
	mainWidgetStack->addWidget(TEMPTBD);
	mainWidgetStack->addWidget(TEMPTBD2);
	
	setCentralWidget(mainWidgetStack);
	
	
	
	
	// ui->setupUi(this);

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


void MainWindow::statusUpdate(QString text) {
	statusBar()->showMessage(text);
	pomdevice.addToTextLog(text);
}
