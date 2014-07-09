#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "pomserialportwidget.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
	Q_OBJECT
	
public:
	explicit MainWindow(QWidget *parent = 0);
	~MainWindow();
	pomSerialPortWidget pomdevice;


private:
    Ui::MainWindow *ui;
	
	QStackedWidget *mainWidgetStack;
};

#endif // MAINWINDOW_H
