#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent)
{
	
	settings = new QSettings(parent);  // Load settings
	
	// Increment the execution count (more for logs than anything else)
	if (settings->contains("ExecutionCount"))
		settings->setValue("ExecutionCount", settings->value("ExecutionCount").toInt() + 1);
	else loadDefaultSettings();  // No settings; let's go ahead and set defaults
		
	settings->sync();  // Save them
	
	
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
	
	
	// Initialize components
	logger = new Logger(this);
	loginWidget = new LoginWidget(this);
	serialWidget = new SerialWidget(this);
	

	
	
	
	/// Widget Stack ///
	mainWidgetStack = new QStackedWidget(this);
	mainWidgetStack->addWidget(loginWidget);
	mainWidgetStack->addWidget(serialWidget);
	//mainWidgetStack->addWidget(TEMPTBD2);
	
	this->setCentralWidget(mainWidgetStack);
	
	
	statusBar()->show();
	
	// Connect local logging aliases
	connect(loginWidget, &LoginWidget::log, logger, &Logger::log);
	connect(serialWidget, &SerialWidget::log, logger, &Logger::log);
	
	// Other connections
	connect(loginWidget, &LoginWidget::loginSuccessful, this, &MainWindow::onLogin);
	connect(quitAct, SIGNAL(triggered()), this, SLOT(quit()));
	connect(resetAct, SIGNAL(triggered()), this, SLOT(reconfigure()));
	
	logger->log(QDateTime::currentDateTime().toString("[yyyy/MM/dd hh:mm:ss] ")+
				"Application initialized! Execution number "+
				QString::number(settings->value("ExecutionCount").toInt())
	);
	connect(logger, &Logger::logUpdated, this, &MainWindow::updateStatus);  // Make log updates show in status bar
	updateStatus("Welcome!");
	
	setWindowTitle("GO3 Treks");
	setMinimumSize(750,450);
	setMaximumSize(750,450);
	resize(750,450);
	
	show();
	
	// Start the processing sequence by checking for login settings.  If we do find settings, try them - login() will take over
	// program execution at that point.  If the credentials are valid, we'll pass right over to serialWidget.
	if (settings->value("login/Valid").toBool())
		loginWidget->login(settings->value("login/Username").toString(), settings->value("login/Password").toString());
	else;  // Else do nothing, just wait at the password screen for user to enter stuff
}


MainWindow::~MainWindow() {
}


void MainWindow::updateStatus(QString text) {
	this->statusBar()->showMessage(text);
}



void MainWindow::onLogin(UserInfo user) {
	
	this->userInfo = user;
	
	settings->setValue("login/Valid", true);
	settings->setValue("login/Username", user.Username);
	settings->setValue("login/Password", user.Password);
	
	mainWidgetStack->setCurrentIndex(mainWidgetStack->currentIndex() + 1);
	
	if (settings->value("serial/Valid").toBool())
		serialWidget->connectToDevice();
	else;  // Else do nothing, just wait for user to configure the serial connection
}



void MainWindow::onSerialSetupComplete() {
	mainWidgetStack->setCurrentIndex(mainWidgetStack->currentIndex() + 1);
}



void MainWindow::onTransmitComplete() {
	mainWidgetStack->setCurrentIndex(mainWidgetStack->currentIndex() + 1);
}



void MainWindow::onCarbonProcessed() {
	mainWidgetStack->setCurrentIndex(mainWidgetStack->currentIndex() + 1);
}



void MainWindow::onUploadComplete() {
	mainWidgetStack->setCurrentIndex(mainWidgetStack->currentIndex() + 1);
}



void MainWindow::reconfigure() {
	logger->log("Resetting configuration.  Application will quit in 10 seconds; simply re-open and start over.");
	
	loadDefaultSettings();
	
	QEventLoop loop;
	QTimer::singleShot(10000, &loop, SLOT(quit()));
	loop.exec();
	
	quit();
}



void MainWindow::loadDefaultSettings() {
	settings->setValue("ExecutionCount", 1);
	settings->setValue("login/Valid", false);
	settings->setValue("login/Username", "");
	settings->setValue("login/Password", "");
	settings->setValue("serial/Valid", false);
	settings->sync();
}



void MainWindow::quit() {
	qApp->exit();
}













