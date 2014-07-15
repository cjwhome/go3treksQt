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

#define NetworkWaitTime 5000 // The time we wait for a network reply in milliseconds

namespace Ui {
class LoginWidget;
}

struct UserInfo {
	bool LoggedIn;
	QString UserID;
	QString Username;
	QString Password;
	QString Email;
	QString RealName;
};

class LoginWidget : public QWidget
{
	Q_OBJECT
	
public:
	explicit LoginWidget(QWidget *parent = 0);
	~LoginWidget();
	bool login(QString username, QString password);
	UserInfo getUserInfo();
	
signals:
	void log(QString text);
	void loginSuccessful(UserInfo userInfo);
	
private slots:
	void updateProgressBar();
	void on_checkLoginButton_clicked();
	
private:
	Ui::LoginWidget *ui;
	UserInfo user;
	int progressBarValue;
};

#endif // LOGINWIDGET_H
