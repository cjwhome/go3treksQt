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

void CarbonDataWidget::processCarbonData(){
    //find file
    QDir microDir;
    QString newPath;
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
               QDir microDir(newPath);
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
           signed int min_secs = 1000000;       //start with a huge difference
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
       }



    }else{
       log("Could not find GO3Treks Folder");
    }
            //If that cannot be found, start at the root directory and search for the folder



}

void CarbonDataWidget::on_pushButton_clicked()
{
    processCarbonData();
}

void CarbonDataWidget::setEndDateTime(QDateTime end){
    endDateTime = end;
}

void CarbonDataWidget::setStartDateTime(QDateTime start){
    startDateTime = start;
}
