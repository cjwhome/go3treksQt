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
	
	resetAct = new QAction("&Reconfigure settings", this);
	resetAct->setStatusTip(tr("Wipe all settings (login credentials, serial setup) and reconfigure"));
	//connect(quitAct, SIGNAL(triggered()), this, SLOT(notImplemented()));
	fileMenu->addAction(resetAct);
	
	quitAct = new QAction("&Quit", this);
	quitAct->setMenuRole(QAction::QuitRole);
	quitAct->setShortcuts(QKeySequence::Quit);
	quitAct->setStatusTip(tr("Exit the program"));
	//connect(quitAct, SIGNAL(triggered()), this, SLOT(notImplemented()));
	fileMenu->addAction(quitAct);
	
	
	
	
	
	// ui->setupUi(this);
	
	logger = new Logger(this);
	
	loginWidget = new LoginWidget(this);
	serialWidget = new SerialWidget(this);
	
	//SerialStep* serialStep = new SerialStep(this);
	
	//connect(pomdevice, pomSerialPortWidget::log, logger, Logger::log);
	
	//if(pomdevice->findPomPort())
	//	pomdevice->pomConnect();
	//qDebug()<<"Found Port";
	
	
	
	/// Widget Stack ///
	mainWidgetStack = new QStackedWidget(this);
	mainWidgetStack->addWidget(loginWidget);
	mainWidgetStack->addWidget(serialWidget);
	//mainWidgetStack->addWidget(TEMPTBD2);
	
	this->setCentralWidget(mainWidgetStack);
	
	
	statusBar()->show();
	
	// Connect local logging aliases
	connect(loginWidget, &LoginWidget::log, logger, &Logger::log);
	
	// Other connections
	connect(loginWidget, &LoginWidget::loginSuccessful, this, &MainWindow::onLogin);
	
	logger->log("Application initialized!");
	connect(logger, &Logger::logUpdated, this, &MainWindow::updateStatus);  // Make log updates show in status bar
	updateStatus("Welcome!");
	
	setWindowTitle("GO3 Treks");
	setMinimumSize(750,500);
	setMaximumSize(750,500);
	resize(750,500);
}


MainWindow::~MainWindow() {
}


void MainWindow::updateStatus(QString text) {
	this->statusBar()->showMessage(text);
}



void MainWindow::onLogin(UserInfo user){
	
	this->userInfo = user;
	
	mainWidgetStack->setCurrentIndex(mainWidgetStack->currentIndex() + 1);
}
























