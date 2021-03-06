#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent)
{
	
    // Get master working path
    dataPath = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation) + "/GO3 Treks Data/";
	QDir dir(dataPath);
	if( ! dir.exists()) dir.mkpath(".");
	QDir::setCurrent(dataPath);

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
	
    syncTimeAct = new QAction("&Sync Time", this);
    syncTimeAct->setStatusTip(tr("Synchronize POM time with Computer Time"));
    fileMenu->addAction(syncTimeAct);
	
	restartLogAct = new QAction("&POM Log Restart", this);
	restartLogAct->setStatusTip(tr("Enables or Disables POM logger restart after transmitting data to keep logger data"));
	restartLogAct->setCheckable(true);
	restartLogAct->setChecked(true);
	fileMenu->addAction(restartLogAct);
	restartPomLogger = true;


	quitAct = new QAction("&Quit", this);
	quitAct->setMenuRole(QAction::QuitRole);
	quitAct->setShortcuts(QKeySequence::Quit);
	quitAct->setStatusTip(tr("Exit the program"));
	//connect(quitAct, SIGNAL(triggered()), this, SLOT(notImplemented()));
	fileMenu->addAction(quitAct);
	

	/// Help Menu ///
	helpMenu = menuBar()->addMenu(tr("&Help"));
	aboutAct = new QAction("&About GO3Treks", this);
	helpMenu->addAction(aboutAct);
	
	
	// Initialize components
	logger = new Logger(this, dataPath);
	loginWidget = new LoginWidget(this, dataPath);
	connectOrFindWidget = new ConnectOrFind(this, dataPath);
	serialWidget = new SerialWidget(this, dataPath);
    ozoneDataWidget = new OzoneDataWidget(this, dataPath);
    carbonDataWidget = new CarbonDataWidget(this, dataPath);
    kmlMakerWidget = new KmlMakerWidget(this, dataPath);
    blogWidget = new BlogWidget(this, dataPath);
	uploadWidget = new UploadWidget(this, dataPath);
	quitWidget = new QuitWidget(this, dataPath);

	/// Widget Stack ///
	mainWidgetStack = new QStackedWidget(this);
	mainWidgetStack->addWidget(loginWidget);
	mainWidgetStack->addWidget(connectOrFindWidget);
	mainWidgetStack->addWidget(serialWidget);
    mainWidgetStack->addWidget(ozoneDataWidget);
    mainWidgetStack->addWidget(carbonDataWidget);
    mainWidgetStack->addWidget(kmlMakerWidget);
    mainWidgetStack->addWidget(blogWidget);
	mainWidgetStack->addWidget(uploadWidget);
	mainWidgetStack->addWidget(quitWidget);
	
	this->setCentralWidget(mainWidgetStack);
	
	
	statusBar()->show();
	
	// Connect local logging aliases
	connect(loginWidget, &LoginWidget::log, logger, &Logger::log);
	connect(connectOrFindWidget, &ConnectOrFind::log, logger, &Logger::log);
	connect(serialWidget, &SerialWidget::log, logger, &Logger::log);
    connect(ozoneDataWidget, &OzoneDataWidget::log, logger, &Logger::log);
    connect(carbonDataWidget, &CarbonDataWidget::log, logger, &Logger::log);
    connect(kmlMakerWidget, &KmlMakerWidget::log, logger, &Logger::log);
    connect(blogWidget, &BlogWidget::log, logger, &Logger::log);
	connect(uploadWidget, &UploadWidget::log, logger, &Logger::log);
	
    // Connect success signals
	connect(loginWidget, &LoginWidget::loginSuccessful, this, &MainWindow::onLogin);
	connect(connectOrFindWidget, &ConnectOrFind::userChoseConnect, this, &MainWindow::onUserChoseConnect);
	connect(connectOrFindWidget, &ConnectOrFind::userChoseFind, this, &MainWindow::onUserChoseFind);
    connect(serialWidget, &SerialWidget::foundPortSuccessful, this, &MainWindow::onFoundPortComplete);
    connect(serialWidget, &SerialWidget::transmitSuccessful, this, &MainWindow::onTransmitComplete);
    connect(ozoneDataWidget, &OzoneDataWidget::processSuccessful, this, &MainWindow::onOzoneProcessed);
    connect(carbonDataWidget, &CarbonDataWidget::processSuccessful, this, &MainWindow::onCarbonProcessed);
    connect(kmlMakerWidget, &KmlMakerWidget::processSuccessful, this, &MainWindow::onKmlProcessed);
	connect(kmlMakerWidget, &KmlMakerWidget::initiateUpload, this, &MainWindow::onBlogRequested);
	connect(blogWidget, &BlogWidget::blogWritten, this, &MainWindow::onBlogWritten);
	connect(uploadWidget, &UploadWidget::uploadSuccessful, this, &MainWindow::onUploadComplete);
	connect(uploadWidget, &UploadWidget::quitSuccessful, this, &MainWindow::onQuitPressed);

	//file menu connections
	connect(quitAct, SIGNAL(triggered()), this, SLOT(quit()));
	connect(resetAct, SIGNAL(triggered()), this, SLOT(reconfigure()));
    connect(syncTimeAct, SIGNAL(triggered()), this, SLOT(synchronizePOMTime()));
	connect(restartLogAct, SIGNAL(triggered()), this, SLOT(setRestartLog()));
    syncTimeAct->setDisabled(true);             //disable this ability until the POM port has been found
	
	//help menu connections
	connect(aboutAct, SIGNAL(triggered()), this, SLOT(displayAbout()));
	
	logger->log(QDateTime::currentDateTime().toString("[yyyy/MM/dd hh:mm:ss] ")+
				"Application initialized!  Treks version " APP_VERSION ". Execution number "+
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
		loginWidget->login(settings->value("login/Email").toString(), settings->value("login/Password").toString());
    else; // Else do nothing, just wait at the password screen for user to enter stuff
}


MainWindow::~MainWindow() {
	settings->sync();
}


void MainWindow::updateStatus(QString text) {
	this->statusBar()->showMessage(text);
}



void MainWindow::onLogin(UserInfo user) {
	
	this->userInfo = user;
	
	settings->setValue("login/Valid", true);
	settings->setValue("login/Email", user.Email);
	settings->setValue("login/Password", user.Password);
	
	mainWidgetStack->setCurrentIndex(mainWidgetStack->currentIndex() + 1);
	
   
}

void MainWindow::onUserChoseConnect(){	
	mainWidgetStack->setCurrentIndex(mainWidgetStack->currentIndex() + 1);
	logger->log("User chose to connect to POM");
	bool found=false;
	while(!found){
	 found = serialWidget->findPomPort();
	}
}

void MainWindow::onUserChoseFind(QFile *fp){
	logger->log("User chose to search for POM file");
	mainWidgetStack->setCurrentIndex(mainWidgetStack->currentIndex() + 2);		//skip serial read
	
	carbonDataWidget->setPomFileLocation(fp);
    ozoneDataWidget->processOzoneData(fp);
}

void MainWindow::onFoundPortComplete(QString portName) {
    logger->log("Found POM port: "+portName+".");
    serialWidget->connectPOM();
    //syncTimeAct->setDisabled(false);
}


void MainWindow::onTransmitComplete(QFile *fp) {
	mainWidgetStack->setCurrentIndex(mainWidgetStack->currentIndex() + 1);
    logger->log("Transmitted Data.");
    carbonDataWidget->setPomFileLocation(fp);
    ozoneDataWidget->processOzoneData(fp);
}

void MainWindow::onOzoneProcessed(){ 
    pomStartTime = ozoneDataWidget->getStartDateTime();
    pomEndTime = ozoneDataWidget->getEndDateTime();
    carbonDataWidget->setStartDateTime(pomStartTime);
    carbonDataWidget->setEndDateTime(pomEndTime);
	blogWidget->setTimes(pomStartTime, pomEndTime);
    mainWidgetStack->setCurrentIndex(mainWidgetStack->currentIndex() + 1);
    logger->log("Processed Ozone Data Successfully.");

    //carbonDataWidget->processCarbonData();
}

void MainWindow::onCarbonProcessed(){
	mainWidgetStack->setCurrentIndex(mainWidgetStack->currentIndex() + 1);
    comboFp = carbonDataWidget->getComboFp();
    logger->log("Generated combo data file");
    kmlMakerWidget->setComboFP(comboFp);
    kmlMakerWidget->createKML();
}

void MainWindow::onKmlProcessed(){
    //mainWidgetStack->setCurrentIndex(mainWidgetStack->currentIndex() + 1);
    kmlFp = kmlMakerWidget->getKMLfp();
    logger->log("Generated KML file");
	
	// Wait a minute (literally)
	/*QTime finalTime = QTime::currentTime().addSecs(60);
	while (finalTime > QTime::currentTime()) {
		QEventLoop loop;
		QTimer::singleShot(100, &loop, SLOT(quit()));
		loop.exec();
	}*/
}
void MainWindow::onBlogRequested() {
	mainWidgetStack->setCurrentIndex(mainWidgetStack->currentIndex() + 1);
}

void MainWindow::onBlogWritten(TrekInfo passedTrekInfo) {
	trekInfo = passedTrekInfo;
	if ( ! kmlMakerWidget->writeMetas(trekInfo.Name, trekInfo.Description)) {
		logger->log("Postprocessing failed, contact GO3 support");
		return;
	}
	mainWidgetStack->setCurrentIndex(mainWidgetStack->currentIndex() + 1);
	uploadWidget->upload(userInfo,kmlFp,trekInfo);
}

void MainWindow::onUploadComplete() {
    //logger->log("well look at that. everything worked. what a miracle. [ICP plays in the distance]");
	logger->log("Well Done!  You have successfully created and uploaded a new blog!");
}

void MainWindow::onQuitPressed(){
	mainWidgetStack->setCurrentIndex(mainWidgetStack->currentIndex() + 1);
	logger->log("Restarting POM logger and exiting program.");
	if(restartPomLogger){
		logger->log("Restarting POM logger now, please connect POM if not connected");
		serialWidget->restartLogging();
	}
	delay();		//delay before exiting
	quit();
}


void MainWindow::returnToStart() {  // For when they've successfully uploaded a KML file and want to make another one
	mainWidgetStack->setCurrentIndex(3);
}



void MainWindow::reconfigure() {
	ReconfigureDialog * dialog = new ReconfigureDialog();
	dialog->exec();
	logger->log("Settings cleared");
	
	loadDefaultSettings();
	
	quit();
}

void MainWindow::synchronizePOMTime() {
    logger->log("Synchronizing the POM time with the computer's system time");
    serialWidget->setPOMTime();
}

void MainWindow::setRestartLog(){
	if(!restartPomLogger){
		restartLogAct->setChecked(true);
		restartPomLogger = true;
	}else{
		restartLogAct->setChecked(false);
		restartPomLogger = false;
	}
}

void MainWindow::loadDefaultSettings() {
	settings->setValue("ExecutionCount", 1);
	settings->setValue("login/Valid", false);
    settings->setValue("login/Email", "");
    settings->setValue("login/Password", "");

	settings->setValue("serial/Valid", false);
	settings->sync();
}



void MainWindow::quit() {
	qApp->exit();
}

void MainWindow::displayAbout(){
	//QMessageBox::about(this, "About GO3Treks", APP_VERSION);
	QMessageBox aboutMessage(this);
	
	aboutMessage.setText("About GO3 Treks Software");
	QString versionString("Version ");
	versionString.append(APP_VERSION);
	aboutMessage.setInformativeText(versionString);
	aboutMessage.setStandardButtons(QMessageBox::Ok);
	aboutMessage.setDefaultButton(QMessageBox::Ok);
	aboutMessage.setFixedWidth(500);
	
	aboutMessage.exec();
}

void MainWindow::delay()
{
    QTime dieTime= QTime::currentTime().addSecs(3);
    while( QTime::currentTime() < dieTime )
    QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
}












