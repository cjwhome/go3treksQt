#include "carbondatawidget.h"
#include "ui_carbondatawidget.h"

CarbonDataWidget::CarbonDataWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CarbonDataWidget)
{
    ui->setupUi(this);
    foundPath = false;

}

CarbonDataWidget::~CarbonDataWidget()
{
    delete ui;
}

bool CarbonDataWidget::processCarbonData(){
    //find file
    QDir microDir;
    QString newPath, current_microAeth_path;
    signed int min_secs = MAX_TIME_DIFFERENCE;       //start with a huge difference
    ui->textBrowser->clear();
    ui->textBrowser->append("POM Start Date/time: " + startDateTime.toString() + ", End Date/Time: " + endDateTime.toString());
        //look in the home directory under /GO3TreksData first for the folder "AE51" in the name
    QString combinedPath = QDir(QDir::home()).filePath("GO3TreksData");
    QDir dir(combinedPath);
    if(dir.exists()){
       log("Found Data Folder.\n");
       if(QDir::setCurrent(combinedPath))
           log("Set path to find microAeth folder.");


       QStringList datFilesAndDirectories = dir.entryList();

       ui->textBrowser->append("\nContents of GO3Treks Dir:");
       for(int i=0; i< datFilesAndDirectories.size();++i){
           ui->textBrowser->append(datFilesAndDirectories.at(i));
           if(datFilesAndDirectories.at(i).contains("AE51")){
               newPath = QDir(QDir::home()).filePath("GO3TreksData/"+datFilesAndDirectories.at(i));
               //QDir microDir(newPath);
               ui->textBrowser->append("Found path:\n"+newPath);
               log("Found path to microAethData: "+newPath);
               foundPath = true;
           }
       }
       if(foundPath){       //Found folder, now look inside for files that have the name with date and time in the same range as our POM start and End data
           QDir::setCurrent(newPath);
           QStringList microAethFiles = microDir.entryList();
           QStringList name_parts_a;
           QString date_with_ext;       //date still has extension attached in first split
           QStringList name_parts_b;
           QString date_time;           //remove extension in second split
           QString foundFilePath;
           ui->textBrowser->append("\nDates of files in microAeth Dir:");

           signed int temp_diff = 0;
           for(int i=0;i<microAethFiles.size();++i)
           {
               //ui->textBrowser->append(microAethFiles.at(i));
               if(microAethFiles.at(i).endsWith(".dat")||microAethFiles.at(i).endsWith(".csv")){
                   name_parts_a = microAethFiles.at(i).split("_");      //first, split into two parts (this should be able to be done with 2 tokens)
                   date_with_ext = name_parts_a.at(1);
                   name_parts_b = date_with_ext.split(".");
                   date_time = name_parts_b.at(0);                      //removed extension (.dat or .csv)
                   QDateTime fileName_dateTime = QDateTime::fromString(date_time, "yyyyMMdd-hhmmss");     //extract the date from the file
                   temp_diff = qAbs(fileName_dateTime.secsTo(startDateTime));                             //absolute value of difference
                   if(temp_diff<min_secs)
                   {
                       min_secs = temp_diff;
                       foundFilePath = microAethFiles.at(i);
                   }
                   ui->textBrowser->append("Date: " + fileName_dateTime.toString()+ " difference - startdate = "+QString::number(min_secs));
               }
           }
           ui->textBrowser->append("\nFound closest microAeth file to POM start time: "+foundFilePath+" with a time difference of "+QString::number(min_secs)+" Seconds.");
           if(min_secs == MAX_TIME_DIFFERENCE){
               log("Could not find any microAeth files whose dates came close.\n");
               ui->textBrowser->append("Could not find any microAeth files whose dates came close.\n");
               return 0;
           }else{
               log("\nFound closest microAeth file to POM start time: "+foundFilePath+" with a time difference of "+QString::number(min_secs)+" Seconds.");
               ui->textBrowser->append("\nFound closest microAeth file to POM start time: "+foundFilePath+" with a time difference of "+QString::number(min_secs)+" Seconds.");
           }
           //Open the pom file and black carbon file for processing
           current_microAeth_path = newPath+"/"+foundFilePath;
           QDir::setCurrent(newPath);
           ui->textBrowser->append("Opening microAeth file located at: " + current_microAeth_path);
           QFile current_microAeth_file(current_microAeth_path);
           if(current_microAeth_file.open(QIODevice::ReadWrite)){
               ui->textBrowser->append("Opened microAeth file successfully.\n");
           }else{
               ui->textBrowser->append("Could not open microAeth file.\n");
               return 0;
           }

           if(pom_fp->open(QIODevice::ReadWrite)){
               ui->textBrowser->append("Opened POM file successfully.");
           }else{
               ui->textBrowser->append("Could not open POM file.");
               current_microAeth_file.close();
               return 0;
           }
           QString newname = "combo-"+startDateTime.toString("ddMMyy_hhmmss")+"-"+endDateTime.toString("hhmmss")+".txt";
           combined_fp.setFileName(newname);
           if(combined_fp.open(QIODevice::ReadWrite))
               ui->textBrowser->append("Opened new file for combining data.");
           else{
               ui->textBrowser->append("Could not open combined file.");
               pom_fp->close();
               current_microAeth_file.close();
               return 0;
           }
           //files are open, now search black carbon file for close match



           //***This is the part that will get the most scrutiny!!*******
           //POM could be set to either 10 seconds or 1 minute averaging, so first check to see if any points are within 10 seconds and then increase to 60 seconds if can't find any at 10
           QString pomLine;
           QStringList pomFields;

           QTextStream pomIn(pom_fp);
           QString microAethLine;
           QStringList microAethFields;
           QTextStream microAethIn(&current_microAeth_file);
           QDateTime pomLineDateTime;
           QDateTime microLineDateTime;
           int max_time_var = MAX_MEASUREMENT_TIME_DIFF_START;
           int combo_lines = 0;     //counts number of lines appended to combo file
           comboFileValid = false;


            //QDataStream out(&combined_fp);   // we will serialize the data into the file
           QTextStream out(&combined_fp);
           //since we are appending a microAeth black carbon measurement to each ozone line, we search the entire ozone file
           //int temp_diff;        //set high first!

           while(!pomIn.atEnd()){
               pomLine = pomIn.readLine();                              //read an entire line
               pomFields = pomLine.split(QRegExp(","));                 //separate all fields separated by commas
               if(pomFields.size() == POM_VALID_LINE_FIELDS){           //make sure it is a measurement line and not other strings
                  if((QString::compare(QString(pomFields[POM_LAT_INDEX]),"0000.00000")!=0)){
                      pomLineDateTime = QDateTime::fromString(pomFields[POM_DATE_INDEX]+pomFields[POM_TIME_INDEX], "dd/MM/yyhh:mm:ss");
                      pomLineDateTime = pomLineDateTime.addYears(100);
                       //now, find a close match black carbon measurement in the microAeth file...
                      //ui->textBrowser->append("POM TIME: "+pomLineDateTime.toString("dd/MM/yy,hh:mm:ss"));
                      temp_diff = 120;      //reset before each search
                       while((temp_diff > max_time_var) && (!microAethIn.atEnd())){       //keep searching until find one or at end of file
                           microAethLine = microAethIn.readLine();
                           microAethFields = microAethLine.split(QRegExp(";"));     //micro uses semicolon

                           if((microAethFields.size() == AETH_VALID_LINE_FIELDS)&&(microAethFields[AETH_TIME_INDEX]!="Time")){
                               microLineDateTime = QDateTime::fromString(microAethFields[AETH_DATE_INDEX]+microAethFields[AETH_TIME_INDEX],"yyyy/MM/ddhh:mm:ss");
                               temp_diff = pomLineDateTime.secsTo(microLineDateTime);                             //absolute value of difference
                               if(temp_diff<0)
                                   temp_diff = microLineDateTime.secsTo(pomLineDateTime);
                               //ui->textBrowser->append("   POM_TIME: "+ pomLineDateTime.toString("dd/MM/yyyy,hh:mm:ss")+", BC_TIME: " + microLineDateTime.toString("dd/MM/yyyy,hh:mm:ss") + ", diff="+QString::number(temp_diff)+" seconds");

                           }
                       }
                       if(temp_diff <= max_time_var){
                           combo_lines++;
                           ui->textBrowser->append("Found a matching black carbon time stamp at time:" + microLineDateTime.toString());
                           pomLine.append(","+microAethFields[AETH_MEASUREMENT_INDEX]);

                           out<<pomLine+"\n";
                       }
                   }
               }
           }

           combined_fp.close();
           current_microAeth_file.close();
           pom_fp->close();
           if(combo_lines>MIN_COMBO_LINES){
               comboFileValid = true;
               emit processSuccessful();
               //QString newname = "combo-"+startDateTime.toString("ddMMyy_hhmmss")+"-"+endDateTime.toString("hhmmss")+".txt";
               /*if(combined_fp.rename(newname)){
                ui->textBrowser->append("Renamed File with start and end dates.");
                log("Renamed combo file.");
               }else{
                   ui->textBrowser->append("Could not rename file.");
                   log("Could not rename combo file - filename possibly already exists?");
               }*/

               return 1;
            }

       }



    }else{
       log("Could not find GO3Treks Folder");
       ui->textBrowser->append("Could not find GO3Treks Folder");
       return 0;
    }
            //If that cannot be found, start at the root directory and search for the folder



}

void CarbonDataWidget::on_pushButton_clicked()
{
    if(processCarbonData())
        log("Yay, processed the carbon data!\n");
}

void CarbonDataWidget::setEndDateTime(QDateTime end){
    endDateTime = end;
}

void CarbonDataWidget::setStartDateTime(QDateTime start){
    startDateTime = start;
}

void CarbonDataWidget::setPomFileLocation(QFile *fp){
    pom_fp = fp;
}

QFile * CarbonDataWidget::getComboFp(){
    return &combined_fp;
}
