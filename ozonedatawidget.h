#ifndef OZONEDATAWIDGET_H
#define OZONEDATAWIDGET_H

#include <QWidget>
#include <QDebug>
#include <QDateTime>
#include <QFile>

//define the positions of fields in the data line
#define LOG_INDEX 0     //log number
#define O3_INDEX 1      //ozone
#define TMP_INDEX 2     //temperature
#define PRS_INDEX 3     //pressure
#define PDV_INDEX 4     //photodiode voltage
#define BTV_INDEX 5     //battery voltage
#define LAT_INDEX 6     //latitude
#define LONG_INDEX 7    //longitude
#define ALT_INDEX 8     //altitude
#define QLTY_INDEX 9
#define DATE_INDEX 10    //date
#define TIME_INDEX 11   //time

#define POM_TOTAL_FIELDS 12

namespace Ui {
class OzoneDataWidget;
}

class OzoneDataWidget : public QWidget
{
    Q_OBJECT

public:
    explicit OzoneDataWidget(QWidget *parent = 0, QString path = "");
    ~OzoneDataWidget();
    void doNothing();
    void processOzoneData(QFile *fp);
    QDateTime getEndDateTime();
    QDateTime getStartDateTime();
    bool getEndDateTimeValid();

signals:
    void log(QString text);
    void processSuccessful();

private:
    Ui::OzoneDataWidget *ui;
	QString dataPath;
    QDateTime startDateTime;
    QDateTime endDateTime;
    QFile *working_file;
    bool endDateTimeValid;
};

#endif // OZONEDATAWIDGET_H
