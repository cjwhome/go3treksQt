#ifndef UPLOADWIDGET_H
#define UPLOADWIDGET_H

#include <QWidget>
#include <QFile>
#include <QJsonObject>
#include <QByteArray>
#include <QVariantMap>
#include <QNetworkAccessManager>
#include <QUrlQuery>
#include <QNetworkReply>
#include <QUrl>
#include <QDesktopServices>
#include <QJsonDocument>
#include "loginwidget.h"
#include "blogwidget.h"

// Define upload progress indicator levels (out of 100)
#define UP_KML_ENCODED		10
#define UP_JSON_ENCODED		20
#define UP_DATA_SENT		30
#define UP_INCREMENT		2  // The speed at which the progress bar goes once the upload has begun; this is added to the progress every 30ms...
#define UP_HOLD_MAX			95  // ...up to this point

#define UploadScriptURL		"http://go3project.com/scripts/upload/TreksUpload.php"


namespace Ui {
class UploadWidget;
}

class UploadWidget : public QWidget
{
	Q_OBJECT
	
public:
	explicit UploadWidget(QWidget *parent = 0);
	~UploadWidget();
	bool upload(UserInfo userInfo, QFile *kmlFile, TrekInfo trekInfo);
	QUrl blogUrl;
	
signals:
	void log(QString text);
	void uploadSuccessful(QUrl blogUrl);
	
private slots:
	void on_viewPostButton_clicked();
	
private:
	Ui::UploadWidget *ui;
};

#endif // UPLOADWIDGET_H
