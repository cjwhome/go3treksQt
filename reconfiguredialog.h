#ifndef RECONFIGUREDIALOG_H
#define RECONFIGUREDIALOG_H

#include <QDialog>

namespace Ui {
class reconfiguredialog;
}

class reconfiguredialog : public QDialog
{
	Q_OBJECT
	
public:
	explicit reconfiguredialog(QWidget *parent = 0);
	~reconfiguredialog();
	
private:
	Ui::reconfiguredialog *ui;
};

#endif // RECONFIGUREDIALOG_H
