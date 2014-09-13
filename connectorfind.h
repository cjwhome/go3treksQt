#ifndef CONNECTORFIND_H
#define CONNECTORFIND_H

#include <QWidget>
#include <QtGui>
#include <QPushButton>
#include <QLabel>
#include <QLayout>
#include <QFileDialog>

namespace Ui {
class ConnectOrFind;
}

class ConnectOrFind : public QWidget
{
	Q_OBJECT
	
public:
	explicit ConnectOrFind(QWidget *parent = 0, QString path = "");
	~ConnectOrFind();
	
signals:
    void log(QString text);
	void userChoseConnect();
	void userChoseFind(QFile *fp);
	
private slots:
	void on_connectPOMButton_clicked();
	void on_findPOMFileButton_clicked();
	
	
private:
	Ui::ConnectOrFind *ui;
	QPushButton *connectPOMButton;
	QPushButton *findPOMFileButton;
	QLabel *recommendLabel;
	QLabel *advancedLabel;
	QLabel *orLabel;
	QString dataPath;
	
};

#endif // CONNECTORFIND_H
