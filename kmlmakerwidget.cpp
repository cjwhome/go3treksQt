#include "kmlmakerwidget.h"
#include "ui_kmlmakerwidget.h"

KmlMakerWidget::KmlMakerWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::KmlMakerWidget)
{
    ui->setupUi(this);
}

KmlMakerWidget::~KmlMakerWidget()
{
    delete ui;
}

//from ddmm.mmmmm to dd.mmmmmm
QString KmlMakerWidget::convertCoordinate(QString str){
    QStringList latParts;
    QString temp;
    QString fracString;
    QString degrees;
    QString newLat;
    int degreeSize;
    //ui->textBrowser->append("string: "+str);

    latParts = str.split(".");
    temp = latParts.at(0);
    degreeSize = temp.size();
    temp.chop(2);      //chop off the minutes
    degrees = temp;

    fracString = str.remove(0,degreeSize-2);
    //ui->textBrowser->append("FracString: " + fracString);
    double fraction = fracString.toDouble();
    //ui->textBrowser->append(QString::number(fraction));
    fraction /= 60;
    fracString = QString::number(fraction);
    fracString.remove(0,1);     //remove leading zero

    //ui->textBrowser->append("Degrees: "+ degrees);
    newLat = degrees + fracString;
    //ui->textBrowser->append("New Lat: "+ newLat);*/

    return newLat;
}



bool KmlMakerWidget::createKML(){
    //QFileInfo fi(*tempFp);
   // QDir::setCurrent(fi.path());
    QFile *newFile = new QFile();
    if(tempFp->open(QIODevice::ReadWrite))
        log("Opened combo file to make KML successfully.");
    else{
        log("Could not open combo file to make KML.");
        return 0;
    }


    //rename the file with kml in place of .txt of combo file
    //QString nameParts = comboFp->fileName().split(".");
    newFile->setFileName(tempFp->fileName().split(".").at(0)+".kml");

    //newFile->setFileName("blah.txt");     //

    if(newFile->open(QIODevice::ReadWrite))
        log("Created and opened new kml file successfully.");
    else{
        log("Unable to create new kml file.");
        return 0;
    }
    QTextStream out(newFile);
    QTextStream in(tempFp);
    QStringList dataFields;
    QString dataLine;
    QString trekName("GO3 Treks");             //get this from the user information?
    QString description = "This is a GO3 Trek"; //we can put the time duration (start and end times) and city/school name in here
    QString latitude;
    QString longitude;
    QString tempString;

    //get gps coordinates of first point to use for the lookat view
    dataLine = in.readLine();                              //read an entire line
    dataFields = dataLine.split(QRegExp(","));
    tempString = dataFields.at(LAT_INDEX);
    //ui->textBrowser->append("Here");
    //separate degrees from rest of string
    latitude = convertCoordinate(tempString);

    tempString = dataFields.at(LONG_INDEX);

    longitude = convertCoordinate(tempString);


    /***********************writing the kml file!!*********************/
    out<<"<?xml version=\"1.0\" encoding=\"UTF-8\"?>";
    out<<"\n";                                                                   //newline
    out<<"<kml xmlns=\"http://www.opengis.net/kml/2.2\">";
    out<<"\n";                                                                   //newline
    out<<"<Document>";
    out<<"\n";
    out<<"<name>";
    out<<trekName;
    out<<"</name>";
    out<<"\n";
    out<<"<description>";
    out<<description;
    out<<"</description>";
    out<<"\n";

    //<!-- This is the icon used for each ozone measurement-->
    out<<"<Style id=\"OzoneIcon\">";
    out<<"\n";
    out<<"  <LabelStyle>";
    out<<"\n";
    out<<"   <color>ffffffff</color>";
    out<<"\n";
    out<<"  </LabelStyle>";
    out<<"\n";
    out<<"   <BalloonStyle>";
    out<<"\n";
    out<<"      <text>$[description]</text>";
    out<<"\n";
    out<<"    </BalloonStyle>";
    out<<"\n";
    out<<"     <IconStyle>";
    out<<"\n";
    out<<"        <color></color>";
    out<<"\n";
    out<<"        <colorMode>normal</colorMode>";
    out<<"\n";
    out<<"        <scale>0.4</scale>";
    out<<"\n";
    out<<"        <Icon>";
    out<<"\n";
    out<<"           <href>http://go3project.com/images/red_point.png</href>";
    out<<"\n";
    out<<"        </Icon>";
    out<<"\n";
    out<<"       <hotSpot x=\"0.5\" y=\"0.5\" xunits=\"fraction\" yunits=\"fraction\" />";
    out<<"\n";
    out<<"     </IconStyle>";
    out<<"\n";
    out<<"   </Style>";
    out<<"\n";

    //<!-- This is the icon used for each black carbon measurement.  Using "BalloonStyle" get rid of the default "Directions to here"  etc. -->
    out<<"<Style id=\"BlackCarbonIcon\">";
    out<<"\n";
    out<<"  <LabelStyle>";
    out<<"\n";
    out<<"   <color>ffffffff</color>";
    out<<"\n";
    out<<"  </LabelStyle>";
    out<<"\n";
    out<<"   <BalloonStyle>";
    out<<"\n";
    out<<"      <text>$[description]</text>";
    out<<"\n";
    out<<"    </BalloonStyle>";
    out<<"\n";
    out<<"     <IconStyle>";
    out<<"\n";
    out<<"        <color>ff00ffff</color>";
    out<<"\n";
    out<<"        <colorMode>normal</colorMode>";
    out<<"\n";
    out<<"        <scale>0.4</scale>";
    out<<"\n";
    out<<"        <Icon>";
    out<<"\n";
    out<<"           <href>http://go3project.com/images/yellow_point.png</href>";
    out<<"\n";
    out<<"        </Icon>";
    out<<"\n";
    out<<"       <hotSpot x=\"0.5\" y=\"0.5\" xunits=\"fraction\" yunits=\"fraction\" />";
    out<<"\n";
    out<<"     </IconStyle>";
    out<<"\n";
    out<<"   </Style>";
    out<<"\n";

    //<!-- Data go into folders.  We will make 2 folders so that each set of data can be turned "on" (displayed) and "off" not displayed.  Both will be displayed initially. -->

    //<!-- Start with a position on the map to look. -->

    out<<"      <LookAt>";
    out<<"\n";
    out<<"        <longitude>";
    out<<longitude;           //<-104.985433;
    out<<"</longitude>";
    out<<"\n";
    out<<"        <latitude>";
    out<<latitude;           //39.736846;
    out<<"</latitude>";
    out<<"\n";
    out<<"        <altitude>";
    out<<500;
    out<<"</altitude>";
    out<<"\n";
    out<<"        <heading>";
    out<<0;
    out<<"</heading>";
    out<<"\n";
    out<<"        <tilt>";
    out<<60;
    out<<"</tilt>";
    out<<"\n";
    out<<"        <range>";
    out<<200;
    out<<"</range>";
    out<<"\n";
    out<<"      </LookAt>";
    out<<"\n";


    out<<"</Document>";
    out<<"\n";
    out<<"</kml>";

    newFile->close();
    tempFp->close();
    return 1;
    //start

}

void KmlMakerWidget::setComboFP(QFile *fp){
    tempFp = fp;
}

QFile * KmlMakerWidget::getKMLfp(){
    return &kmlFp;
}
