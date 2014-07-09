#ifndef LOGINWIDGET_H
#define LOGINWIDGET_H

#include <QWidget>

namespace Ui {
class LoginWidget;
}

class LoginWidget : public QWidget
{
	Q_OBJECT
	
public:
	explicit LoginWidget(QWidget *parent = 0);
	~LoginWidget();
	
signals:
	void log(QString text);
	
private slots:
	void on_pushButton_clicked();
	
private:
	Ui::LoginWidget *ui;
};

#endif // LOGINWIDGET_H
