#include "loginwidget.h"
#include "ui_loginwidget.h"

LoginWidget::LoginWidget(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::LoginWidget)
{
	ui->setupUi(this);
	ui->label->setAlignment(Qt::AlignCenter);
	ui->label->setText("Login with your GO<sub>3</sub> Social Network account");
	ui->loginProgressBar->setMaximum(NetworkWaitTime);
	
	user.LoggedIn = false;
}

LoginWidget::~LoginWidget()
{
	delete ui;
}

bool LoginWidget::login(QString username, QString password) {
	
	log("Attempting login (takes a while)...");
	
	
	
	QNetworkAccessManager *nwam = new QNetworkAccessManager;
	
	QNetworkRequest r (QUrl(LoginScriptURL));
	r.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
	
	QString requestString("{\"Username\":\"" + username + "\",\"Password\":\"" + password + "\"}");
	
	// Build the POST data to be fed into the request
	QByteArray postData;
	QUrlQuery q;
    q.addQueryItem("Request", requestString);
    postData = q.query(QUrl::FullyEncoded).toUtf8();
	
	QNetworkReply *reply = nwam->post(r, postData);  // Actually send the request
	//while ( reply->waitForReadyRead(1));  // Block for up to 30s or until we've gotten our response
	
	ui->loginProgressBar->setValue(0);
	progressBarValue = 0;
	QTimer *timer = new QTimer(this);
	connect(timer, SIGNAL(timeout()), this, SLOT(updateProgressBar()));
	timer->start(10);
	while (progressBarValue < NetworkWaitTime) {
		QEventLoop loop;
		QTimer::singleShot(100, &loop, SLOT(quit()));
		loop.exec();
	}
	timer->stop();
	ui->loginProgressBar->setValue(0);
	
	if ( ! reply->isFinished()) {
		log("Request failed!  Error: timed out with no reply.");
		return false;
	}

	if (reply->error() != QNetworkReply::NoError) {
		log("Request failed!  Error: "+reply->errorString());
		return false;
	}
	//log("Returned "+reply->readAll());
	
	QJsonDocument response = QJsonDocument::fromJson(reply->readAll());
	if (response.isNull()) {
		log("Request failed!  Error: The response could not be parsed as valid JSON.");
		return false;
	}
	
	QJsonObject resOb = response.object();
	
	
	if (resOb["Response"].toString() != "Success") {
		QJsonArray errors = resOb["Errors"].toArray();
		QString errorBuf;
		for (int i=0; i<errors.count(); i++) {
			if (i != 0) errorBuf.append(", ");
			errorBuf.append(errors[i].toString());
		}
		log("Login failed!  "+errorBuf+".");
		return false;
	}
	
	QJsonObject dataBuf = resOb["Data"].toObject();
	user.LoggedIn = true;
	user.UserID = dataBuf["UserID"].toString();
	user.Username = dataBuf["Username"].toString();
	user.Email = dataBuf["UserEmail"].toString();
	user.RealName = dataBuf["UserDisplayName"].toString();
	user.Password = password;
	
	log("Logged in!  Hello, "+user.RealName+"!");
	
	return true;
}

void LoginWidget::on_checkLoginButton_clicked()
{
	if (ui->usernameBox->text().isEmpty()) {
		log("You must enter a username!");
		return;
	}
	if (ui->passwordBox->text().isEmpty()) {
		log("You must enter an email!");
		return;
	}
	
	ui->checkLoginButton->setEnabled(false);
	ui->usernameBox->setEnabled(false);
	ui->passwordBox->setEnabled(false);
	
    bool status = login(ui->usernameBox->text(), ui->passwordBox->text());
	
	ui->checkLoginButton->setEnabled(true);
	ui->usernameBox->setEnabled(true);
	ui->passwordBox->setEnabled(true);
	
	if ( ! status) return;
	
	emit loginSuccessful(user);
}

UserInfo LoginWidget::getUserInfo() {
	return this->user;
}

void LoginWidget::updateProgressBar() {
	progressBarValue += 10;
	ui->loginProgressBar->setValue(progressBarValue);
}


















void LoginWidget::on_usernameBox_returnPressed()
{
    on_checkLoginButton_clicked();
}

void LoginWidget::on_passwordBox_returnPressed()
{
    on_checkLoginButton_clicked();
}
