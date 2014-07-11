#include "loginwidget.h"
#include "ui_loginwidget.h"

LoginWidget::LoginWidget(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::LoginWidget)
{
	ui->setupUi(this);
	ui->label->setAlignment(Qt::AlignCenter);
	ui->label->setText("Login with your GO<sub>3</sub> Social Network account");
}

LoginWidget::~LoginWidget()
{
	delete ui;
}

bool LoginWidget::checkLogin(QString username, QString password){
	
	// TODO: Display an error in these cases
	if (username.isEmpty()) {
		log("You must enter a username!");
		return false;
	}
	if (password.isEmpty()) {
		log("You must enter an email!");
		return false;
	}
	
	log("Attempting login (takes a while)...");
	
	QNetworkAccessManager *nwam = new QNetworkAccessManager;
	
	QNetworkRequest r (QUrl("http://go3project.com/scripts/user/SE_CheckLogin.php"));
	r.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
	
	QString requestString("{\"Username\":\"" + username + "\",\"Email\":\"" + password + "\"}");
	
	// Build the POST data to be fed into the request
	QByteArray postData;
	QUrlQuery q;
    q.addQueryItem("Request", requestString);
    postData = q.query(QUrl::FullyEncoded).toUtf8();
	
	QNetworkReply *reply = nwam->post(r, postData);  // Actually send the request
	//while ( reply->waitForReadyRead(1));  // Block for up to 30s or until we've gotten our response
	
	QEventLoop loop;
	QTimer::singleShot(10000, &loop, SLOT(quit()));
	loop.exec();
	
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
		for (int i=0; i++; i<errors.count()) {
			if (i != 0) errorBuf.append(", ");
			errorBuf.append(errors[1].toString());
		}
		log("Login failed!  "+errorBuf);
	}
	else log("Login successful!");
	
}

void LoginWidget::on_checkLoginButton_clicked()
{
    checkLogin(ui->usernameBox->text(), ui->passwordBox->text());
}
