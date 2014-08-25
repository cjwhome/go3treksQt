#include "reconfiguredialog.h"
#include "ui_reconfiguredialog.h"

reconfiguredialog::reconfiguredialog(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::reconfiguredialog)
{
	ui->setupUi(this);
}

reconfiguredialog::~reconfiguredialog()
{
	delete ui;
}
