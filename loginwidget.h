#ifndef LOGINWIDGET_H
#define LOGINWIDGET_H

#include <QWidget>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrl>
#include <QUrlQuery>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QTimer>

namespace Ui {
class LoginWidget;
}

class LoginWidget : public QWidget
{
	Q_OBJECT
	
public:
	explicit LoginWidget(QWidget *parent = 0);
	~LoginWidget();
	bool checkLogin(QString username, QString password);
	
signals:
	void log(QString text);
	
private slots:
	
	void on_checkLoginButton_clicked();
	
private:
	Ui::LoginWidget *ui;
};

#endif // LOGINWIDGET_H
