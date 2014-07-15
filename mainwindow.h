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

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
	Q_OBJECT
	
public:
	explicit MainWindow(QWidget *parent = 0);
	~MainWindow();
	
	// File Menu Actions
	QAction *resetAct;
	QAction *quitAct;
	
	//pomSerialPortWidget pomdevice;
	Logger *logger;
	LoginWidget *loginWidget;
	SerialWidget *serialWidget;

public slots:
	void updateStatus(QString text);
	void onLogin(UserInfo user);

private:
    Ui::MainWindow *ui;
	
	QStackedWidget *mainWidgetStack;
	QMenu *fileMenu;
	
	UserInfo userInfo;
	
};

#endif // MAINWINDOW_H
