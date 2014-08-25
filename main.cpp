#include "mainwindow.h"
#include "defines.h"
#include <QtGui>
#include <QApplication>

#define TREKS_VERSION "1.0"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    //w.show();
	
	QCoreApplication::setOrganizationName("Global Ozone Project");
	QCoreApplication::setOrganizationDomain("go3project.com");
	QCoreApplication::setApplicationName("GO3 Treks Builder");
	QCoreApplication::setApplicationVersion(APP_VERSION);

    return a.exec();
}
