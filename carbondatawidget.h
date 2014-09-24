#ifndef CARBONDATAWIDGET_H
#define CARBONDATAWIDGET_H

#define MAX_TIME_DIFFERENCE 100000
#define MAX_MEASUREMENT_TIME_DIFF_START 15  //start with 10 seconds max, then switch to 60 in case the POM is set to 1 min averaging
#define MAX_MEASUREMENT_TIME_DIFF_SECOND 60
#define MIN_COMBO_LINES 1               //minimum number of lines to create a valid combo file

#define POM_VALID_LINE_FIELDS 12        //log#(0),O3(1),T(2),P(3),Vpdv(4),Vbat(5),Lat(6),Long(7),Alt(8),Qual(9),Date(10,Time(11)
//POM feild indexes
#define POM_LOG_INDEX 0                     //log number
#define POM_O3_INDEX 1                      //ozone
#define POM_TMP_INDEX 2                     //temperature
#define POM_PRS_INDEX 3                     //pressure
#define POM_PDV_INDEX 4                     //photodiode voltage
#define POM_BTV_INDEX 5                     //battery voltage
#define POM_LAT_INDEX 6                     //latitude
#define POM_LONG_INDEX 7                    //longitude
#define POM_ALT_INDEX 8                     //altitude
#define POM_QLTY_INDEX 9
#define POM_DATE_INDEX 10                   //date
#define POM_TIME_INDEX 11                   //time

#define AETH_VALID_LINE_FIELDS 10       //Date(yyyy/MM/dd)(0);Time(1);Ref(2);Sen(3);ATN(4);Flow(5);Temp(6);Status(7);Battery(8);BC(9)
//microAeth feild indexes
#define AETH_DATE_INDEX 0                   //date
#define AETH_TIME_INDEX 1                   //time
#define AETH_MEASUREMENT_INDEX 9

#define MIN_LAT_SIZE 9      //minimum number of characters in the latitude string (for checking bad transmitted data from pom gps)

#define ALLOW_JUST_OZONE false

#define SIZE_OF_AETH_DATE_PART 15	//20140909-130100 (yyyymmdd-hhmmss) but might have a -number so need to remove the -number

#define BYTES_PER_LINE 50       //approximate number of bytes per line in a POM file

#include <QWidget>
#include <QDir>
#include <QString>
#include <QFile>
#include <QDateTime>
#include <QTextStream>
#include <QDataStream>


namespace Ui {
class CarbonDataWidget;
}

class CarbonDataWidget : public QWidget
{
    Q_OBJECT

public:
    explicit CarbonDataWidget(QWidget *parent = 0, QString path = "");
    ~CarbonDataWidget();
    void setEndDateTime(QDateTime end);
    void setStartDateTime(QDateTime start);
    void setPomFileLocation(QFile *fp);
    bool processCarbonData();
    QFile * getComboFp();
   // bool averageCarbonData();

signals:
    void log(QString text);
    void processSuccessful();

private slots:
    void on_pushButton_clicked();

private:
    Ui::CarbonDataWidget *ui;
	QString dataPath;
	QString bcPath;
    QDateTime startDateTime;
    QDateTime endDateTime;
    QFile *pomFp;
    QFile comboFp;
    bool foundPath;
    bool foundFile;
    bool comboFileValid;
	bool foundNewStartTime;

};

#endif // CARBONDATAWIDGET_H
