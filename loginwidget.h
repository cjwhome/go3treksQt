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
	QString Email;
	QString Password;
	QString RealName;
};

class LoginWidget : public QWidget
{
	Q_OBJECT
	
public:
	explicit LoginWidget(QWidget *parent = 0, QString path = "");
	~LoginWidget();
	bool login(QString email, QString password);
	UserInfo getUserInfo();
	
signals:
	void log(QString text);
	void loginSuccessful(UserInfo userInfo);
	
private slots:
	void on_checkLoginButton_clicked();
	void on_emailBox_returnPressed();  // Just calls on_checkLoginButton_clicked();
	void on_passwordBox_returnPressed();  // Just calls on_checkLoginButton_clicked();
	
private:
	Ui::LoginWidget *ui;
	QString dataPath;
	UserInfo user;
};

#endif // LOGINWIDGET_H
