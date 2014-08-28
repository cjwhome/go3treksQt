#include "uploadwidget.h"
#include "ui_uploadwidget.h"

UploadWidget::UploadWidget(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::UploadWidget)
{
	ui->setupUi(this);
}

UploadWidget::~UploadWidget()
{
	delete ui;
}

bool UploadWidget::upload(UserInfo userInfo, QFile *kmlFile, TrekInfo trekInfo) {
	
	// Prepare stuff
	ui->viewPostButton->setEnabled(false);
	ui->uploadAnotherButton->setEnabled(false);
	ui->uploadProgressBar->setValue(0);
	blogUrl.clear();
	
	// Load the data into a QVariantMap (QMap<QString,QVariant>)
	QVariantMap requestData;
	
	requestData.insert("Email", userInfo.Email);
	requestData.insert("Password", userInfo.Password);
	requestData.insert("TrekName", trekInfo.Name);
	requestData.insert("TrekCity", trekInfo.City);
	requestData.insert("TrekDescription", trekInfo.Description);
	requestData.insert("TrekStartTime", trekInfo.StartTime);
	requestData.insert("TrekEndTime", trekInfo.EndTime);
	
	// Base-64 encode KML data
	requestData.insert("KML", kmlFile->readAll().toBase64());
	ui->uploadProgressBar->setValue(UP_KML_ENCODED);
	
	// Convert QVariantMap to QJsonObject to QJsonDocument to QByteArray
	QByteArray request = QJsonDocument(QJsonObject::fromVariantMap(requestData)).toBinaryData();
	ui->uploadProgressBar->setValue(UP_JSON_ENCODED);
	
	// Get things going
	QNetworkAccessManager *nwam = new QNetworkAccessManager;
	
	QNetworkRequest r (QUrl(UploadScriptURL));
	r.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
	
	// Turn the requestData into a POST header entry
	QUrlQuery q;
    q.addQueryItem("Request", request);
    request = q.query(QUrl::FullyEncoded).toUtf8();
	
	// Actually send the request
	QNetworkReply *reply = nwam->post(r, request);
	ui->uploadProgressBar->setValue(UP_DATA_SENT);
	
	// Wait 60 seconds or until we've got a response, whichever comes first
	// Also update the progress bar at least every 30ms, meaning it updates above 30fps and won't appear to jump
	int timePassed = 0;
	int progressBarValue = ui->uploadProgressBar->value();
	while ( ! reply->isFinished() && timePassed < 60000) {
		QEventLoop loop;
		QTimer::singleShot(30, &loop, SLOT(quit()));
		loop.exec();
		timePassed += 30;
		if (progressBarValue < UP_HOLD_MAX) {
			progressBarValue += UP_INCREMENT;
			ui->uploadProgressBar->setValue(progressBarValue);
		}
	}
	
	if ( ! reply->isFinished()) {
		log(QString("Network upload failure; KML saved at '").append(kmlFile->fileName()).append("'"));
		log("Connection timed out.  Contact GO3 to manually upload the file.");
		return false;
	}
	
	if (reply->error() != QNetworkReply::NoError) {
		log(QString("Network upload failure; KML saved at '").append(kmlFile->fileName()).append("'"));
		log("Request failed!  Error: "+reply->errorString());
		log("Network error.  Contact GO3 to manually upload the file.");
		return false;
	}
	
	// Parse the reply as JSON
	QJsonDocument response = QJsonDocument::fromJson(reply->readAll());
	if (response.isNull()) {
		log(QString("Network upload failure; KML saved at '").append(kmlFile->fileName()).append("'"));
		log("The response could not be parsed as valid JSON.");
		log("Network error.  Contact GO3 to manually upload the file.");
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
		log(QString("Network upload failure; KML saved at '").append(kmlFile->fileName()).append("'"));
		log("Server said the following: "+errorBuf+".");
		log("Server error.  Contact GO3 to manually upload the file.");
		return false;
	}
	
	QJsonObject dataBuf = resOb["Data"].toObject();
	blogUrl = dataBuf["BlogURL"].toString();
	log("Upload successful!  Check out your blog post to see the trek.");
	ui->viewPostButton->setEnabled(true);
	emit uploadSuccessful(blogUrl);
	return true;
}

void UploadWidget::on_viewPostButton_clicked()
{
    if (blogUrl.isEmpty()) log("You haven't yet uploaded a blog post!");
	else QDesktopServices::openUrl(blogUrl);
}
