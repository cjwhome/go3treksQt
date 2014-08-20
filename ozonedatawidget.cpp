#include "ozonedatawidget.h"
#include "ui_ozonedatawidget.h"

OzoneDataWidget::OzoneDataWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::OzoneDataWidget)
{
    ui->setupUi(this);
}

OzoneDataWidget::~OzoneDataWidget()
{
    delete ui;
}

void OzoneDataWidget::processOzoneData(QFile *fp){
    //working_file = fp;
    QStringList fields;
    QDateTime tempDateTime;     //used for comparison

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
            for(int a=0;a<fields.size();++a)
            {
               ui->fileData->append("Line:" + QString::number(i) +"(field "+ QString::number(a) + "):" + fields[a] + " ");
            }
            ui->fileData->append("\n");

            /*****look for valid start and end times*********/
            //first, check to be sure there are gps coordinate in the line (latitude will always have data other than zeros if there was a connection)

           if(fields.size()==POM_TOTAL_FIELDS){
               if((QString::compare(QString(fields[LAT_INDEX]),"0000.00000")!=0)){  //comment this line if we want all data
                    if(!foundStartTime){
                        startDateTime = QDateTime::fromString(fields[DATE_INDEX]+fields[TIME_INDEX], "dd/MM/yyhh:mm:ss");
                        foundStartTime = true;
                        log("Found Start time");
                        startDateTime = startDateTime.addYears(100);
                        ui->startTimeOutput->setText(startDateTime.toString());
                        endDateTime = startDateTime;        //do this here for a starting point for the endtime because it at least has to be greater than the starttime

                    }else{      //found the start time, now keep looking for the end time

                        tempDateTime = QDateTime::fromString(fields[DATE_INDEX]+fields[TIME_INDEX], "dd/MM/yyhh:mm:ss");
                        tempDateTime = tempDateTime.addYears(100);
                        if(tempDateTime > endDateTime){          //replace if greater
                            endDateTime = tempDateTime;
                            foundEndTime = true;
                        }
                    }
               }
           }

           i++;
        }   //while end

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
