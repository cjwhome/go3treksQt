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
#include <QPixmap>
#include "defines.h"

#define LoginScriptURL "http://go3project.com/scripts/user/SE_CheckLogin.php"

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
	void on_checkLoginButton_clicked();
	void on_usernameBox_returnPressed();  // Just calls on_checkLoginButton_clicked();
	void on_passwordBox_returnPressed();  // Just calls on_checkLoginButton_clicked();
	
private:
	Ui::LoginWidget *ui;
	UserInfo user;
};

#endif // LOGINWIDGET_H
