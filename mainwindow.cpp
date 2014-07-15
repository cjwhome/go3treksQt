#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent)
{
	
	//TEMPTBD = new QListWidget(this);
	
	
	
	
	
	
	/////////////////////
	/// MENU BUILDING ///
	/////////////////////
	
	
	/// File Menu ///
	fileMenu = menuBar()->addMenu(tr("&File"));
	
	quitAct = new QAction(tr("&Quit", "File menu"), this);
	quitAct->setMenuRole(QAction::QuitRole);
	quitAct->setShortcuts(QKeySequence::Quit);
	quitAct->setStatusTip(tr("Exit the program"));
	//connect(quitAct, SIGNAL(triggered()), this, SLOT(notImplemented()));
	fileMenu->addAction(quitAct);
	
	
	
	
	
	// ui->setupUi(this);
	
	logger = new Logger(this);
	
	loginWidget = new LoginWidget(this);
	
	//SerialStep* serialStep = new SerialStep(this);
	
	//connect(pomdevice, pomSerialPortWidget::log, logger, Logger::log);
	
	//if(pomdevice->findPomPort())
	//	pomdevice->pomConnect();
	//qDebug()<<"Found Port";
	
	
	
	/// Widget Stack ///
	mainWidgetStack = new QStackedWidget(this);
	mainWidgetStack->addWidget(loginWidget);
	//mainWidgetStack->addWidget(TEMPTBD2);
	
	this->setCentralWidget(mainWidgetStack);
	
	
	statusBar()->show();
	
	
	
	connect(loginWidget, &LoginWidget::log, logger, &Logger::log);
	
	logger->log("Application initialized!");
	
	connect(logger, &Logger::logUpdated, this, &MainWindow::updateStatus);
	updateStatus("Welcome!");
	
	setWindowTitle(tr("GO3 Treks"));
	setMinimumSize(750,500);
	setMaximumSize(750,500);
	resize(750,500);
}


MainWindow::~MainWindow() {
}


void MainWindow::updateStatus(QString text) {
	this->statusBar()->showMessage(text);
}



void MainWindow::onLogin(UserInfo userInfo){
	
	
	
}
























