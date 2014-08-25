#ifndef UPDATEREQUIREDDIALOG_H
#define UPDATEREQUIREDDIALOG_H

#include <QDialog>

namespace Ui {
class UpdateRequiredDialog;
}

class UpdateRequiredDialog : public QDialog
{
	Q_OBJECT
	
public:
	explicit UpdateRequiredDialog(QWidget *parent = 0);
	~UpdateRequiredDialog();
	
private:
	Ui::UpdateRequiredDialog *ui;
};

#endif // UPDATEREQUIREDDIALOG_H
