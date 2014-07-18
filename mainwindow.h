#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "pomserialportwidget.h"
#include "logger.h"
#include "loginwidget.h"
#include "serialwidget.h"
#include <QStackedWidget>
#include <QMenu>
#include <QAction>
#include <QMenuBar>
#include <QStatusBar>
#include <QSettings>
#include <QDateTime>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
	Q_OBJECT
	
public:
	explicit MainWindow(QWidget *parent = 0);
	~MainWindow();
	
	QSettings *settings;
	
	// File Menu Actions
	QAction *resetAct;
	QAction *quitAct;
	
	Logger *logger;
	LoginWidget *loginWidget;
	SerialWidget *serialWidget;

public slots:
	void updateStatus(QString text);
	void onLogin(UserInfo user);  // Called as soon as the user has successfully logged in
	void onSerialSetupComplete();  // Called as soon as the POM serial configuration is complete
	void onTransmitComplete();  // Called as soon as the instrument has successfully transmitted data
	void onCarbonProcessed();  // Called as soon as a carbon file has been found & processed
	void onUploadComplete();  // Called as soon as the KML is made and uploaded
	void returnToStart();
	void reconfigure();  // Called by the "Reconfigure" menu option
	void quit();

private:
    Ui::MainWindow *ui;
	
	QStackedWidget *mainWidgetStack;
	QMenu *fileMenu;
	
	UserInfo userInfo;
	
	void loadDefaultSettings();  // Called by the "Reconfigure" menu option and when setting default settings
	
};

#endif // MAINWINDOW_H
