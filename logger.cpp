#include "logger.h"

Logger::Logger(QObject *parent) :
	QObject(parent)
{
	logHasBeenWritten = false;
}

void Logger::log(QString text) {
	if (text.isEmpty()) return;
	
	emit logUpdated(text);
	logs.append(text);
}

bool Logger::writeLog() {
	
	// Get log location
    //QString path = QStandardPaths::writableLocation(QStandardPaths::DataLocation);

	
    logFile.setFileName("GO3Treks_log.log");

    if ( ! logFile.open(QIODevice::ReadWrite)) return false;  // The Text flag localizes line endings
	
	// Move to end
	if ( ! logFile.seek(logFile.size())) return false;
	
	int x = logFile.write(logs.join("\n	").append("\n\n").toUtf8());
	
	logFile.close();
	
	if (x == -1) return false;
	else {
		logHasBeenWritten = true;
        qDebug() <<"Wrote log file";
		return true;
	}
}


Logger::~Logger() {
	if ( ! logHasBeenWritten) writeLog();
}
