#include "ozonedatawidget.h"
#include "ui_ozonedatawidget.h"

OzoneDataWidget::OzoneDataWidget(QWidget *parent, QString path) :
    QWidget(parent),
    ui(new Ui::OzoneDataWidget)
{
    ui->setupUi(this);
	dataPath = path;
}

OzoneDataWidget::~OzoneDataWidget()
{
    delete ui;
}

void OzoneDataWidget::processOzoneData(QFile *fp){
    //working_file = fp;
    QStringList fields;
    QDateTime tempDateTime;			//used for comparison
	QDateTime previousDateTime;		//used to see how far apart the consecutive points are to determine if we need to designate 
	
	startTimeCounter = 0;

    bool foundStartTime = false;
    bool foundEndTime = false;

    if(fp->open(QIODevice::ReadWrite)){
        log("Opened ozone File");
        QString line;
        QTextStream in(fp);
        endDateTimeValid = false;
        int i=0;
        //create a default start time and end time that will be all
        while (!in.atEnd()) {
            line = in.readLine();
            fields = line.split(QRegExp(","));
           // for(int a=0;a<fields.size();++a)
          //  {
           //    ui->fileData->append("Line:" + QString::number(i) +"(field "+ QString::number(a) + "):" + fields[a] + " ");
          //  }
           // ui->fileData->append("\n");

            /*****look for valid start and end times*********/
            //first, check to be sure there are gps coordinate in the line (latitude will always have data other than zeros if there was a connection)
			signed int temp_diff = 0;
           if(fields.size()==POM_TOTAL_FIELDS){
			   if((QString::compare(QString(fields[LAT_INDEX]),"0000.00000")!=0)&&(fields[LAT_INDEX].toDouble())&&fields[LONG_INDEX].toDouble()){  //comment this line if we want all data
                    if(!startTimeCounter){
						startDateTime = QDateTime::fromString(fields[DATE_INDEX]+fields[TIME_INDEX], "dd/MM/yyhh:mm:ss");
						startDateTime = startDateTime.addYears(100);			//for some reason, it assumes the date is 19XX instead of 20XX
                        startTimeList.append(startDateTime);
						startTimeCounter++;		
                        foundStartTime = true;
                        log("Found first start time");
                        previousDateTime = tempDateTime;
                        //ui->startTimeOutput->setText(startDateTime.toString());
                        endDateTime = startDateTime;        //do this here for a starting point for the endtime because it at least has to be greater than the starttime

                    }else{      //found the first start time, now keep looking for the end time and other start times

                        tempDateTime = QDateTime::fromString(fields[DATE_INDEX]+fields[TIME_INDEX], "dd/MM/yyhh:mm:ss");
                        tempDateTime = tempDateTime.addYears(100);
						temp_diff = previousDateTime.secsTo(tempDateTime); 
						previousDateTime = tempDateTime;
						if(temp_diff > ADD_ANOTHER_START_TIME_LIMIT){
							
							startTimeList.append(tempDateTime);
							startTimeCounter++;
							log("Found another potential start time: " + tempDateTime.toString("dd/MM/yyyy,hh:mm:ss"));
						}
                        if(tempDateTime > endDateTime){          //replace if greater
                            endDateTime = tempDateTime;
                            foundEndTime = true;
                        }
                    }
               }
           }

           i++;
        }   //while end
		
		if(startTimeCounter){
			log("Found multiple start times");
			//startDateTime = displayStartTimes(startTimeList);
		}

        if(foundStartTime&&foundEndTime){
            if(startDateTime.addSecs(10)<endDateTime){      //make sure there is a difference
                endDateTimeValid = true;
                emit processSuccessful();
            }else
                log("No difference between start and end times\n");
        }else
            log("Error in finding start and end times\n");
        ui->endTimeOutput->setText(endDateTime.toString());
        fp->close();
    }


    //qDebug()<<"Processing Ozone Data";
}

QList<QDateTime> OzoneDataWidget::getStartDateTimeList(){
	return startTimeList;
}

int OzoneDataWidget::getStartTimeCount(){
	return startTimeCounter;
}

QDateTime OzoneDataWidget::getStartDateTime(){
    return startDateTime;
}

QDateTime OzoneDataWidget::getEndDateTime(){
    return endDateTime;
}



bool OzoneDataWidget::getEndDateTimeValid(){
    return endDateTimeValid;

}

void OzoneDataWidget::doNothing(){
    qDebug()<<"doing nothing in ozonedata widget\n";
}
