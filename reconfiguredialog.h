#ifndef RECONFIGUREDIALOG_H
#define RECONFIGUREDIALOG_H

#include <QDialog>

namespace Ui {
class ReconfigureDialog;
}

class ReconfigureDialog : public QDialog
{
	Q_OBJECT
	
public:
	explicit ReconfigureDialog(QWidget *parent = 0);
	~ReconfigureDialog();
	
private:
	Ui::ReconfigureDialog *ui;
};

#endif // RECONFIGUREDIALOG_H
