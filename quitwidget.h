#ifndef QUITWIDGET_H
#define QUITWIDGET_H

#include <QWidget>

namespace Ui {
class QuitWidget;
}

class QuitWidget : public QWidget
{
	Q_OBJECT
	
public:
	explicit QuitWidget(QWidget *parent = 0, QString path = "");
	~QuitWidget();
	
private:
	Ui::QuitWidget *ui;
};

#endif // QUITWIDGET_H
