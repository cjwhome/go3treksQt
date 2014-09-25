#include "connectorfind.h"
#include "ui_connectorfind.h"

ConnectOrFind::ConnectOrFind(QWidget *parent, QString path) :
	QWidget(parent),
	ui(new Ui::ConnectOrFind)
{
	ui->setupUi(this);
	
	dataPath = path;
	
	connectPOMButton = new QPushButton("Connect to POM");
	connectPOMButton->setMinimumSize(100,50);
	connectPOMButton->setStatusTip("Press this if you just went on a trek and want to download the data from the POM");

	orLabel = new QLabel("OR");
	
	findPOMFileButton = new QPushButton("Find POM file");
	findPOMFileButton->setMinimumSize(100,50);
	findPOMFileButton->setStatusTip("PRESS ONLY if there was a problem when you tried to upload and you already have downloaded the trek from the POM (RawPom.txt)");
	
	recommendLabel = new QLabel("(Recommended)");
	recommendLabel->setAlignment(Qt::AlignCenter);
	advancedLabel = new QLabel("(Advanced)");
	advancedLabel->setAlignment(Qt::AlignCenter);
	QHBoxLayout *hlayout = new QHBoxLayout;		//lays out from left to right
	QVBoxLayout *vlayout = new QVBoxLayout;		//lays out from top to bottom
	
	QGridLayout *glayout = new QGridLayout;
	glayout->addWidget(connectPOMButton,0,0);
	glayout->addWidget(orLabel,0,1);
	glayout->addWidget(findPOMFileButton,0,2);
	glayout->addWidget(recommendLabel,1,0);
	glayout->addWidget(advancedLabel,1,2);
	glayout->setAlignment(Qt::AlignHCenter);
	glayout->setHorizontalSpacing(50);
	vlayout->addSpacerItem(new QSpacerItem(1,250));
	vlayout->addLayout(glayout);
	vlayout->addSpacerItem(new QSpacerItem(1,250));
	
	/*hlayout->addSpacerItem(new QSpacerItem(150,1));
	hlayout->addLayout(vlayout);
	hlayout->addSpacerItem(new QSpacerItem(150,1));*/
	connect(connectPOMButton, SIGNAL(clicked()), this, SLOT(on_connectPOMButton_clicked()));
	connect(findPOMFileButton, SIGNAL(clicked()), this, SLOT(on_findPOMFileButton_clicked()));
	
	setLayout(vlayout);
	
	
}

ConnectOrFind::~ConnectOrFind()
{
	delete ui;
}

void ConnectOrFind::on_connectPOMButton_clicked(){
	connectPOMButton->setEnabled(false);
	emit userChoseConnect();
}

void ConnectOrFind::on_findPOMFileButton_clicked(){
	findPOMFileButton->setEnabled(false);
	//QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), QStandardPaths::writableLocation(QStandardPaths::DesktopLocation) + "/GO3 Treks Data/", tr("Data Files (*.txt);"));
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), dataPath, tr("Data Files (*.txt);"));
    //QString fileName = "C:/Users/Craig/Desktop/GO3 Treks Data/RawPom.txt";

    POMFile.setFileName(fileName);
    /*if(POMFile.open(QIODevice::ReadWrite | QIODevice::Truncate)){
        log("Opened POM file successfully");
    }else
        log("Could not open POM file");

    POMFile.close();*/
    emit userChoseFind(&POMFile);
}
