#include "logger.h"

Logger::Logger(QObject *parent) :
	QObject(parent)
{
}

void Logger::log(QString text) {
	if (text.isEmpty()) return;
	logs.append(text);
	emit logUpdated(text);
}
