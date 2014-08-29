#ifndef BLOGWIDGET_H
#define BLOGWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QDateTime>

namespace Ui {
class BlogWidget;
}

struct TrekInfo {
	QString Name;
	QString City;
	QString Description;
	QDateTime StartTime;
	QDateTime EndTime;
};

class BlogWidget : public QWidget
{
    Q_OBJECT

public:
    explicit BlogWidget(QWidget *parent = 0);
    ~BlogWidget();
	TrekInfo trekInfo;
	void setTimes(QDateTime start, QDateTime end);

signals:
    void log(QString text);
    void blogWritten(TrekInfo trekInfo);

private slots:
	void on_uploadButton_clicked();
	
private:
    Ui::BlogWidget *ui;
};

#endif // BLOGWIDGET_H
