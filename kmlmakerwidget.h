#ifndef KMLMAKERWIDGET_H
#define KMLMAKERWIDGET_H

#define LOG_INDEX 0                     //log number
#define O3_INDEX 1                      //ozone
#define TMP_INDEX 2                     //temperature
#define PRS_INDEX 3                     //pressure
#define PDV_INDEX 4                     //photodiode voltage
#define BTV_INDEX 5                     //battery voltage
#define LAT_INDEX 6                     //latitude
#define LONG_INDEX 7                    //longitude
#define ALT_INDEX 8                     //altitude
#define QLTY_INDEX 9
#define DATE_INDEX 10                   //date
#define TIME_INDEX 11                   //time
#define BC_INDEX 12                     //black carbon

#define MAX_POINTS 8192                 //maximum stored points in POM

#include <QWidget>
#include <QFile>
#include <QDir>
#include <QFileInfo>
#include <QTextStream>
#include <qxmlstream.h>
#include <QDesktopServices>
#include <QUrl>

namespace Ui {
class KmlMakerWidget;
}

class KmlMakerWidget : public QWidget
{
    Q_OBJECT

public:
    explicit KmlMakerWidget(QWidget *parent = 0);
    ~KmlMakerWidget();
    void setComboFP(QFile *fp);
    bool createKML();
    QFile * getKMLfp();

signals:
    void log(QString text);
    void kmlSuccessful();

private:
    Ui::KmlMakerWidget *ui;
    QFile *tempFp;
    QFile kmlFp;
    QString convertCoordinate(QString str);
};

#endif // KMLMAKERWIDGET_H
