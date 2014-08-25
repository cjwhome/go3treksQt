#include "updaterequireddialog.h"
#include "ui_updaterequireddialog.h"

UpdateRequiredDialog::UpdateRequiredDialog(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::UpdateRequiredDialog)
{
	ui->setupUi(this);
}

UpdateRequiredDialog::~UpdateRequiredDialog()
{
	delete ui;
}
