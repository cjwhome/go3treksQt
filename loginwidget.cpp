#include "loginwidget.h"
#include "ui_loginwidget.h"
#include "updaterequireddialog.h"

LoginWidget::LoginWidget(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::LoginWidget)
{
	ui->setupUi(this);
	
	ui->label->setAlignment(Qt::AlignCenter);
	ui->label->setText("Login with your GO<sub>3</sub> Social Network account");
	
	QPixmap pix(":/TreksLogo");
	QSize targetSize = QSize(this->width()-150, this->height());
	pix = pix.scaled(
				targetSize,
				Qt::KeepAspectRatio,
				Qt::SmoothTransformation);
	ui->imageHolder->setPixmap(pix);
	ui->imageHolder->setMinimumSize(targetSize);
	//ui->imageHolder->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
	//ui->horizontalLayout_2->update();
	
	user.LoggedIn = false;
}

LoginWidget::~LoginWidget()
{
	delete ui;
}

bool LoginWidget::login(QString username, QString password) {
	
	if (user.LoggedIn) return true;  // They're already logged in, this is a duplicate call.
	
	log("Logging in...");
	
	ui->usernameBox->setText(username);
	ui->passwordBox->setText(password);  // TODO: Remove this when we switch to a password system
	
	ui->checkLoginButton->setEnabled(false);
	ui->usernameBox->setEnabled(false);
	ui->passwordBox->setEnabled(false);
	ui->loginProgressBar->setMaximum(0);  // Sets the progress bar to "working" animation
	
	// Get things going
	QNetworkAccessManager *nwam = new QNetworkAccessManager;
	
	QNetworkRequest r (QUrl(LoginScriptURL));
	r.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
	
	// Build the POST data to be fed into the request
	QString requestString("{\"Username\":\"" + username + "\",\"Password\":\"" + password + "\"}");
	QByteArray postData;
	QUrlQuery q;
    q.addQueryItem("Request", requestString);
    postData = q.query(QUrl::FullyEncoded).toUtf8();
	
	QNetworkReply *reply = nwam->post(r, postData);  // Actually send the request
	
	// Wait 30 seconds or until we've got a response, whichever comes first
	int timePassed = 0;
	while ( ! reply->isFinished() && timePassed < 30000) {
		QEventLoop loop;
		QTimer::singleShot(100, &loop, SLOT(quit()));
		loop.exec();
		timePassed += 100;
	}
	
	ui->checkLoginButton->setEnabled(true);
	ui->usernameBox->setEnabled(true);
	ui->passwordBox->setEnabled(true);
	ui->loginProgressBar->setMaximum(1);  // Reset progress bar to blank state
	
	if ( ! reply->isFinished()) {
		log("Request failed!  Error: Connection timed out.");
		return false;
	}
	
	if (reply->error() != QNetworkReply::NoError) {
		log("Request failed!  Error: "+reply->errorString());
		return false;
	}
	
	// Parse the reply as JSON
	QJsonDocument response = QJsonDocument::fromJson(reply->readAll());
	if (response.isNull()) {
		log("Request failed!  Error: The response could not be parsed as valid JSON.");
		return false;
	}
	QJsonObject resOb = response.object();
	
	// Check for errors from the server
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
	
	// No errors, the user has logged in.  Store their information and call home.
	QJsonObject dataBuf = resOb["Data"].toObject();
	user.LoggedIn = true;
	user.UserID = dataBuf["UserID"].toString();
	user.Username = dataBuf["Username"].toString();
	user.Email = dataBuf["UserEmail"].toString();
	user.RealName = dataBuf["UserDisplayName"].toString();
	user.Password = password;
	
	if (QString(APP_VERSION).toDouble() < dataBuf["MinVersion"].toDouble()) {
		UpdateRequiredDialog * dialog = new UpdateRequiredDialog();
		log("Program out of date!  Version "+dataBuf["MinVersion"].toString()+" required.");
		dialog->exec();
		qApp->exit();
		return false;
	}
	else {
		log("Logged in.  Hello, "+user.RealName+"!");
		
		emit loginSuccessful(user);
		
		return true;
	}
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
	
    login(ui->usernameBox->text(), ui->passwordBox->text());
}

UserInfo LoginWidget::getUserInfo() {
	return this->user;
}


















void LoginWidget::on_usernameBox_returnPressed()
{
    on_checkLoginButton_clicked();
}

void LoginWidget::on_passwordBox_returnPressed()
{
    on_checkLoginButton_clicked();
}
