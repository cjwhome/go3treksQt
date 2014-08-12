#ifndef CARBONDATAWIDGET_H
#define CARBONDATAWIDGET_H

#include <QWidget>
#include <QDir>
#include <QString>
#include <QFile>
#include <QDateTime>

namespace Ui {
class CarbonDataWidget;
}

class CarbonDataWidget : public QWidget
{
    Q_OBJECT

public:
    explicit CarbonDataWidget(QWidget *parent = 0);
    ~CarbonDataWidget();
    void setEndDateTime(QDateTime end);
    void setStartDateTime(QDateTime start);
    void processCarbonData();

signals:
    void log(QString text);
    void processSuccessful();

private slots:
    void on_pushButton_clicked();

private:
    Ui::CarbonDataWidget *ui;
    QDateTime startDateTime;
    QDateTime endDateTime;
    bool foundPath;

};

#endif // CARBONDATAWIDGET_H
