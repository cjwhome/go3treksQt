#include "kmlmakerwidget.h"
#include "ui_kmlmakerwidget.h"

KmlMakerWidget::KmlMakerWidget(QWidget *parent, QString path) :
QWidget(parent),
ui(new Ui::KmlMakerWidget)
{
	ui->setupUi(this);
	dataPath = path;
}

KmlMakerWidget::~KmlMakerWidget()
{
	delete ui;
}

//from ddmm.mmmmm to dd.mmmmmm
QString KmlMakerWidget::convertCoordinate(QString str) {
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
	//fracString = QString::number(fraction);
	fracString.setNum(fraction, 'f', 7);
	fracString.remove(0,1);     //remove leading zero
	
	//ui->textBrowser->append("Degrees: "+ degrees);
	newLat = degrees + fracString;
	//ui->textBrowser->append("New Lat: "+ newLat);*/
	
	return newLat;
}



bool KmlMakerWidget::createKML() {
	
	ui->uploadButton->setEnabled(false);
	ui->openButton->setEnabled(false);
	
	struct MeasurementPoint
	{
		int ozone;
		int blackCarbon;
		QString lon;
		QString lat;
		QString alt;
		QString time;
	};
	
    //QFileInfo fi(*kmlFp);
	// QDir::setCurrent(fi.path());
	//QFile *newFile = new QFile();
    if(comboFp->open(QIODevice::ReadOnly | QIODevice::Text))
		log("Opened combo file to make KML successfully.");
	else {
		log("Could not open combo file to make KML.");
		return 0;
	}
	
	QString kmlFileName = comboFp->fileName();
	kmlFileName.chop(4);
	kmlFileName += ".kml";
	
    kmlFp.setFileName(kmlFileName);
	if(kmlFp.open(QIODevice::ReadWrite | QIODevice::Truncate | QIODevice::Text))
		log("Created and opened new kml file successfully.");
	else {
		log("Unable to create new kml file.");
		return 0;
	}
	QTextStream out(&kmlFp);
    QTextStream in(comboFp);
	QStringList dataFields;
	QString dataLine;
	QString lookAtLatitude;
	QString lookAtLongitude;
	QString tempString;
	MeasurementPoint mPoint[MAX_POINTS];
	
	out.setIntegerBase(10);
	out.setRealNumberNotation(QTextStream::FixedNotation);
	out.setRealNumberPrecision(7);
	
	long i = 0;
	double longitudeTotal = 0;
	double latitudeTotal = 0;

	//determine the range for the lookat points
	dataLine = in.readLine();	//skip first point due to potential bad (TODO: remove this because it is crap!)
	while(!in.atEnd()){
		dataLine = in.readLine();
		dataFields = dataLine.split(QRegExp(","));
		tempString = dataFields.at(LAT_INDEX);
		
		mPoint[i].lat = convertCoordinate(tempString);
		latitudeTotal += mPoint[i].lat.toDouble();
		tempString = dataFields.at(LONG_INDEX);
		mPoint[i].lon = convertCoordinate(tempString);
		longitudeTotal += mPoint[i].lon.toDouble();
		i++;
	}
	
		
	int array_length = i;
	int min = 0;
	int j;
	int a;
	QString maxLat, minLat;
	QString maxLong, minLong;
	QString temp;
	//double temp_double_a, temp_double_b;
	
	for(a = 0;a < array_length;a++)	//run sort algorithm on array of measurements
	{
		min = a;
		for(j = a+1;j <= array_length;j++){
			//temp_double_a = mPoint[j].lat.toDouble();
			//temp_double_b = mPoint[min].lat.toDouble();
			if(mPoint[j].lat.toDouble() < mPoint[min].lat.toDouble()) min = j;
			//swap array[min] and array[j]
			temp = mPoint[min].lat;
			mPoint[min].lat = mPoint[a].lat;
			mPoint[a].lat = temp;
		}				
	}
	//lookAtLatitude = mPoint[(array_length/2)-1].lat;	//take median value
	minLat = mPoint[1].lat;
	maxLat = mPoint[array_length-1].lat;
	lookAtLatitude.setNum((minLat.toDouble()+maxLat.toDouble())/2, 'f', 7);		//calculate averages for the midpoint
	
	
	for(a = 0;a < array_length;a++)	//run sort algorithm on array of measurements
	{
		min = a;
		for(j = a+1;j <= array_length;j++){
			//temp_double_a = mPoint[j].lat.toDouble();
			//temp_double_b = mPoint[min].lat.toDouble();
			if(mPoint[j].lon.toDouble() < mPoint[min].lon.toDouble()) min = j;
			//swap array[min] and array[j]
			temp = mPoint[min].lon;
			mPoint[min].lon = mPoint[a].lon;
			mPoint[a].lon = temp;
		}				
	}
	
	minLong = mPoint[1].lon;
	maxLong = mPoint[array_length-2].lon;
	
	lookAtLongitude.setNum((minLong.toDouble()+maxLong.toDouble())/2, 'f', 7);		//average the max and min values to calculate the midpoint
	
	qDebug()<<"Min latitude: "<<minLat;
	qDebug()<<"Max latitude: "<<maxLat;
	qDebug()<<"Min longtitude: "<<minLong;
	qDebug()<<"Max longitude: "<<maxLong;
	
	qDebug()<<"Median latitude: "<<lookAtLatitude;
	qDebug()<<"Median longitude: "<<lookAtLongitude;
	
	//display all of the lat and longs to see if they are sorted
	/*for(a=0;a<array_length;a++){
		qDebug()<<"Long["<<a<<"]="<<mPoint[a].lon;
		//qDebug()<<"Lat["<<a<<"]="<<mPoint[a].lat;
	}*/
	
	//find the longitude and latitude difference in radians
	
	double longDiffRad, latDiffRad;
	double longDistance, latDistance;
	double range;
	int heading;
	
    latDiffRad = (maxLat.toDouble() - minLat.toDouble())/360*2*3.14159;
	if(minLong.toDouble() > maxLong.toDouble())
		longDiffRad = (minLong.toDouble() - maxLong.toDouble())/360*2*3.14159;
	else
		longDiffRad = (maxLong.toDouble() - minLong.toDouble())/360*2*3.14159;
	
	qDebug()<<"Long Diff Rad: "<<longDiffRad;
	qDebug()<<"Lat Diff Rad: "<<latDiffRad;
	
	//calculate the E-W distance
	longDistance = 6371*qCos(lookAtLatitude.toDouble()/360*2*3.14159)*longDiffRad;
	latDistance = 6371*latDiffRad;
	
	qDebug()<<"Long Distance: "<<longDistance;
	qDebug()<<"Lat Distance: "<<latDistance;
	
	if(longDistance>latDistance)
		heading = 0;
	else
		heading = 90;
	
	if(longDistance>latDistance)
		range = longDistance*RANGE_MULTIPLIER;
	else 
		range = latDistance*RANGE_MULTIPLIER;
	
	if(range < RANGE_MINIMUM)
		range = RANGE_MINIMUM;
	
	
	#if AVERAGE_BLACK_CARBON
	double bc_array[6];
	double bc_avg;
	#endif
	
	i=0;		//reset before loop
	in.seek(0);
	while(!in.atEnd()) {
		dataLine = in.readLine();					
		dataFields = dataLine.split(QRegExp(","));
		
		tempString = dataFields.at(O3_INDEX);
		mPoint[i].ozone = tempString.toDouble();
		tempString = dataFields.at(LONG_INDEX);
		mPoint[i].lon = convertCoordinate(tempString);
		tempString = dataFields.at(LAT_INDEX);
		mPoint[i].lat = convertCoordinate(tempString);
		mPoint[i].alt = dataFields.at(ALT_INDEX);
		mPoint[i].time = dataFields.at(TIME_INDEX);
		tempString = dataFields.at(BC_INDEX); 
#if AVERAGE_BLACK_CARBON
		bc_array[0]=bc_array[1];
		bc_array[1]=bc_array[2];
		bc_array[2]=bc_array[3];
		bc_array[3]=bc_array[4];
		bc_array[4]=bc_array[5];
		bc_array[5]=tempString.toDouble();
		bc_avg = (bc_array[0] + bc_array[1] + bc_array[2] + bc_array[3] + bc_array[4] + bc_array[5])/6;
		bc_avg *= BC_SCALING_FACTOR;
		if(i<5)
			mPoint[i].blackCarbon = 0;
		else
			mPoint[i].blackCarbon = bc_avg;
		
#else
		mPoint[i].blackCarbon = tempString.toDouble();
#endif
		i++;
	}
	log("Created "+QString::number(i)+" placemarks in KML file.");
	ui->textBrowser->append("Created "+QString::number(i)+" placemarks in KML file.");
	
	/***********************writing the kml file!!*********************/
	out<<"<?xml version=\"1.0\" encoding=\"UTF-8\"?>";
	out<<"\n";                                                                   //newline
	out<<"<kml xmlns=\"http://www.opengis.net/kml/2.2\">";
	out<<"\n";                                                                   //newline
	out<<"<!-- This KML file automatically generated by GO3 Treks version " APP_VERSION " -->";
	out<<"\n";
	out<<"<Document>";
	out<<"\n";
	out<<"<name>";
	info.nameLocation = out.pos();
	out<<"</name>";
	out<<"\n";
	out<<"<description><![CDATA[";
	info.descriptionLocation = out.pos();
	out<<"]]></description>";
	out<<"\n";
	
	//!-- This is the ozone LineStyle.  LineStyle sets the width and color of the line.  PolyStyle sets the transparency of the otherwise grey fill beneath the extruded line.  The first two digits of the color determine the transparency.  Perfectly clear is "00".  Perfectly solid is "ff". The color is an inverted hex color bbggrr (BGR instead of RGB in HTML) -->
	out<<"<Style id=\"OzoneFence\">";
	out<<"\n";
	out<<"      <LineStyle>";
	out<<"\n";
	out<<"        <width>3</width>";
	out<<"\n";
	out<<"        <color>ff0000ff</color>";
	out<<"\n";
	out<<"      </LineStyle>";
	out<<"\n";
	out<<"      <PolyStyle>";
	out<<"\n";
	out<<"      <color>990000ff</color>";
	out<<"\n";
	out<<"      <colorMode>normal</colorMode>";
	out<<"\n";
	out<<"    </PolyStyle>";
	out<<"\n";
	out<<"    </Style>";
	out<<"\n";
	
	out<<"<Style id=\"BlackCarbonFence\">";
	out<<"\n";
	out<<"      <LineStyle>";
	out<<"\n";
	out<<"        <width>3</width>";
	out<<"\n";
	out<<"        <color>ff00ffff</color>";
	out<<"\n";
	out<<"      </LineStyle>";
	out<<"\n";
	out<<"     <PolyStyle>";
	out<<"\n";
	out<<"      <color>9900ffff</color>";
	out<<"\n";
	out<<"      <colorMode>normal</colorMode>";
	out<<"\n";
	out<<"    </PolyStyle>";
	out<<"\n";
	out<<"    </Style>";
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
	out<<lookAtLongitude;           //<-104.985433;
	out<<"</longitude>";
	out<<"\n";
	out<<"        <latitude>";
	out<<lookAtLatitude;           //39.736846;
	out<<"</latitude>";
	out<<"\n";
	out<<"        <altitude>";
	out<<500;
	out<<"</altitude>";
	out<<"\n";
	out<<"        <heading>";
	out<<heading;
	out<<"</heading>";
	out<<"\n";
	out<<"        <tilt>";
	out<<60;
	out<<"</tilt>";
	out<<"\n";
	out<<"        <range>";
	out<<range;
	out<<"</range>";
	out<<"\n";
	out<<"      </LookAt>";
	out<<"\n";
	
	//<!-- The following opens the main folder so that both Ozone and Black Carbon Folders appear -->
	out<<"<open>1</open>";
	out<<"\n";
	//<!-- Folder for ozone data. -->
	out<<"<Folder>";
	out<<"\n";
	//<!-- Placemarks -->
	out<<"      <name>Ozone</name>";
	out<<"\n";
	out<<"      <description>Ozone Trek Data</description>";
	out<<"\n";
	//<!-- Do LookAt again in each folder so that you can click on that folder and reposition in case you have moved around on the map. -->
	
	out<<"      <LookAt>";
	out<<"\n";
	out<<"        <longitude>";
	out<<lookAtLongitude;
	out<<"</longitude>";
	out<<"\n";
	out<<"        <latitude>";
	out<<lookAtLatitude;
	out<<"</latitude>";
	out<<"\n";
	out<<"        <altitude>";
	out<<500;
	out<<"</altitude>";
	out<<"\n";
	out<<"        <heading>";
	out<<heading;
	out<<"</heading>";
	out<<"\n";
	out<<"        <tilt>60</tilt>";
	out<<"\n";
	out<<"        <range>";
	out<<range;
	out<<"</range>";
	out<<"\n";
	out<<"      </LookAt>";
	out<<"\n";
	//<!-- Draw "Ozone Fence" -->
	out<<"<Placemark>";
	out<<"\n";
	out<<"          <name>Ozone</name>";
	out<<"\n";
	out<<"          <visibility>1</visibility>";
	out<<"\n";
	out<<"          <styleUrl>#OzoneFence</styleUrl>";
	out<<"\n";
	out<<"          <LineString>";
	out<<"\n";
	out<<"            <extrude>1</extrude>";
	out<<"\n";
	out<<"            <tessellate>0</tessellate>";
	out<<"\n";
	out<<"            <altitudeMode>relativeToGround</altitudeMode>";
	out<<"\n";
	out<<"                <coordinates>";
	out<<"\n";
	
	//<!-- Coordinates and ozone measurements:  Longitude, Latitude, Ozone -->
	
	int count;
	for (count=0;count<i;count++) {
		out<<mPoint[count].lon;
		out<<",";
		out<<mPoint[count].lat;
		out<<",";
		out<<mPoint[count].ozone;
		out<<"\n";
	}
	
	out<<"                </coordinates>";
	out<<"\n";
	out<<"     </LineString>";
	out<<"\n";
	out<<"</Placemark>";
	out<<"\n";
	
	for (count=0;count<i;count++) {
		out<<"<Placemark>";
		out<<"\n";
		out<<"        <name></name>";
		out<<"\n";
		out<<"        <Point>";
		out<<"\n";
		out<<"        <altitudeMode>relativeToGround</altitudeMode>";
		out<<"\n";
		out<<"          <coordinates>";
		out<<mPoint[count].lon;
		out<<",";
		out<<mPoint[count].lat;
		out<<",";
		out<<mPoint[count].ozone;
		out<<"</coordinates>";
		out<<"\n";
		out<<"        </Point>";
		out<<"\n";
		out<<"        <styleUrl>#OzoneIcon</styleUrl>";
		out<<"\n";
		out<<"        <description>";
		out<<"Ozone = ";
		out<<mPoint[count].ozone;
		out<<" ppb\n";
		out<<"Time: ";
		out<<mPoint[count].time;
		out<<"</description>";
		out<<"\n";
		out<<"     </Placemark>";
		out<<"\n";
	}
	
	out<<"</Folder>";
	
	
	//<!-- The following opens the main folder so that both Ozone and Black Carbon Folders appear -->
	out<<"<open>1</open>";
	out<<"\n";
	//<!-- Folder for black carbon data. -->
	out<<"<Folder>";
	out<<"\n";
	//<!-- Placemarks -->
	out<<"      <name>Black Carbon</name>";
	out<<"\n";
	out<<"      <description>Black Carbon Trek Data</description>";
	out<<"\n";
	//<!-- Do LookAt again in each folder so that you can click on that folder and reposition in case you have moved around on the map. -->
	
	out<<"      <LookAt>";
	out<<"\n";
	out<<"        <longitude>";
	out<<lookAtLongitude;
	out<<"</longitude>";
	out<<"\n";
	out<<"        <latitude>";
	out<<lookAtLatitude;
	out<<"</latitude>";
	out<<"\n";
	out<<"        <altitude>";
	out<<500;
	out<<"</altitude>";
	out<<"\n";
	out<<"        <heading>";
	out<<heading;
	out<<"</heading>";
	out<<"\n";
	out<<"        <tilt>60</tilt>";
	out<<"\n";
	out<<"        <range>";
	out<<range;
	out<<"</range>";
	out<<"\n";
	out<<"      </LookAt>";
	out<<"\n";
	//<!-- Draw "Black Carbon Fence" -->
	out<<"<Placemark>";
	out<<"\n";
	out<<"          <name>Black Carbon</name>";
	out<<"\n";
	out<<"          <visibility>1</visibility>";
	out<<"\n";
	out<<"          <styleUrl>#BlackCarbonFence</styleUrl>";
	out<<"\n";
	out<<"          <LineString>";
	out<<"\n";
	out<<"            <extrude>1</extrude>";
	out<<"\n";
	out<<"            <tessellate>0</tessellate>";
	out<<"\n";
	out<<"            <altitudeMode>relativeToGround</altitudeMode>";
	out<<"\n";
	out<<"                <coordinates>";
	//out<<"\n";
	
	//<!-- Coordinates and black carbon measurements:  Longitude, Latitude, black carbon -->
	
	for (count=0;count<i;count++) {
		out<<mPoint[count].lon;
		out<<",";
		out<<mPoint[count].lat;
		out<<",";
		if (mPoint[count].blackCarbon<0)	//we don't want to use negative values for altitude!
			out<<0.0;
		else
			out<<mPoint[count].blackCarbon;
		out<<"\n";
	}
	out<<"                </coordinates>";
	out<<"\n";
	out<<"     </LineString>";
	out<<"\n";
	out<<"</Placemark>";
	out<<"\n";
	
	for (count=0;count<i;count++) {
		out<<"<Placemark>";
		out<<"\n";
		out<<"        <name></name>";
		out<<"\n";
		out<<"        <Point>";
		out<<"\n";
		out<<"        <altitudeMode>relativeToGround</altitudeMode>";
		out<<"\n";
		out<<"          <coordinates>";
		out<<mPoint[count].lon;
		out<<",";
		out<<mPoint[count].lat;
		out<<",";
		if (mPoint[count].blackCarbon<0)	//we don't want to use negative values for altitude!
			out<<0.0;
		else
			out<<mPoint[count].blackCarbon;
		out<<"</coordinates>";
		out<<"\n";
		out<<"        </Point>";
		out<<"\n";
		out<<"        <styleUrl>#BlackCarbonIcon</styleUrl>";
		out<<"\n";
		out<<"        <description>";
		out<<"Black Carbon = ";
		out<<mPoint[count].blackCarbon/BC_SCALING_FACTOR;
		out<<" ng/m3\n";
		out<<"Time: ";
		out<<mPoint[count].time;
		out<<"</description>";
		out<<"\n";
		out<<"     </Placemark>";
		out<<"\n";
	}
	
	out<<"</Folder>";
	out<<"\n";
	out<<"</Document>";
	out<<"\n";
	out<<"</kml>";
	
	comboFp->close();
    kmlFp.close();
	
	ui->textBrowser->append("\n\nKML generation is complete.  You now have the opportunity to explore your trek so that you can write a more accurate description.  When you're finished, click the 'Upload' button to upload your trek to the GO3 website.");
	ui->openButton->setEnabled(true);
	
    emit processSuccessful();
	return 1;
}

void KmlMakerWidget::setComboFP(QFile *fp){
    comboFp = fp;
}

QFile * KmlMakerWidget::getKMLfp(){
    return &kmlFp;
}

bool KmlMakerWidget::writeMetas(QString name, QString desc) {
	return true;
    if ( ! kmlFp.open(QIODevice::ReadWrite | QIODevice::Text)) {
		log("Could not open KML file for meta insertion");
		return false;
	}
    if ( ! kmlFp.seek(info.nameLocation)) {
		log("Could not seek to name location");
        kmlFp.close();
		return false;
	}
    if ( ! kmlFp.write(name.toHtmlEscaped().toUtf8())) {
		log("Could not insert name");
        kmlFp.close();
		return false;
	}
    if ( ! kmlFp.seek(info.descriptionLocation)) {
		log("Could not seek to description location");
        kmlFp.close();
		return false;
	}
    if ( ! kmlFp.write(desc.toHtmlEscaped().toUtf8())) {
		log("Could not insert description");
        kmlFp.close();
		return false;
	}
    kmlFp.close();
	
	log("Metadata successfully injected");
	return true;
}

void KmlMakerWidget::on_openButton_clicked()
{
    QDesktopServices::openUrl(QUrl("file:///"+dataPath+kmlFp.fileName()));
	ui->openButton->setEnabled(false);
	ui->uploadButton->setEnabled(true);
	uploadPressed = false;
	
	// Wait a minute (literally)
	/*QTime finalTime = QTime::currentTime().addSecs(60);
	while (finalTime > QTime::currentTime() && !uploadPressed) {
		QEventLoop loop;
		QTimer::singleShot(100, &loop, SLOT(quit()));
		loop.exec();
	}
	qApp->alert(this, 0);*/
}

void KmlMakerWidget::on_uploadButton_clicked()
{
    ui->uploadButton->setEnabled(false);
	uploadPressed = true;
	
    if ( ! kmlFp.open(QIODevice::ReadWrite)) {
		ui->textBrowser->append("You must first close any application using the KML file!");
		return;
	}
    kmlFp.close();
	
	emit initiateUpload();
}
