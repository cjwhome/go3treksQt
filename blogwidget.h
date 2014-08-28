#ifndef BLOGWIDGET_H
#define BLOGWIDGET_H

#include <QWidget>

namespace Ui {
class BlogWidget;
}

struct TrekInfo {
	QString Name;
	QString City;
	QString State;
	QString Description;
	uint StartTime;
	uint EndTime;
};

class BlogWidget : public QWidget
{
    Q_OBJECT

public:
    explicit BlogWidget(QWidget *parent = 0);
    ~BlogWidget();

signals:
    void log(QString text);
    void blogSuccessful();

private:
    Ui::BlogWidget *ui;
};

#endif // BLOGWIDGET_H
