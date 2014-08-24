#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "logger.h"
#include "loginwidget.h"
#include "serialwidget.h"
#include "ozonedatawidget.h"
#include "carbondatawidget.h"
#include "kmlmakerwidget.h"
#include "blogwidget.h"
#include <QTime>
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
    QAction *syncTimeAct;
	
	Logger *logger;
	LoginWidget *loginWidget;
	SerialWidget *serialWidget;
    OzoneDataWidget *ozoneDataWidget;
    CarbonDataWidget *carbonDataWidget;
    KmlMakerWidget *kmlMakerWidget;
    BlogWidget *blogWidget;

public slots:
	void updateStatus(QString text);
	void onLogin(UserInfo user);  // Called as soon as the user has successfully logged in
    void onFoundPortComplete(QString portName);

    void onTransmitComplete(QFile *fp);  // Called as soon as the instrument has successfully transmitted data
    void onOzoneProcessed();    // Called as soon as ozone data has been processed (validated gps data and start and end date and times
    void onCarbonProcessed();   // Called as soon as a carbon file has been found & processed
    void onKmlProcessed();
    void onUploadComplete();    // Called as soon as the KML is made and uploaded
    void reconfigure();         // Called by the "Reconfigure" menu option
    void synchronizePOMTime();
	void quit();
    void delay();

private:
    Ui::MainWindow *ui;
	
	QStackedWidget *mainWidgetStack;
	QMenu *fileMenu;
	
	UserInfo userInfo;
	
	void loadDefaultSettings();  // Called by the "Reconfigure" menu option and when setting default settings
    QDateTime pomStartTime;
    QDateTime pomEndTime;
    QFile *comboFp;               //file pointer for the combo file
    QFile *kmlFp;
	
};

#endif // MAINWINDOW_H
