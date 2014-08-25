#include "reconfiguredialog.h"
#include "ui_reconfiguredialog.h"

ReconfigureDialog::ReconfigureDialog(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::ReconfigureDialog)
{
	ui->setupUi(this);
}

ReconfigureDialog::~ReconfigureDialog()
{
	delete ui;
}
