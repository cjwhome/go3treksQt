#ifndef LOGGER_H
#define LOGGER_H

#include <QObject>
#include <QStringList>
#include <QStandardPaths>
#include <QDir>
#include <QFile>

class Logger : public QObject
{
	Q_OBJECT
public:
	explicit Logger(QObject *parent = 0);
	~Logger();
	bool writeLog();
	
signals:
	void logUpdated(QString text);
	
public slots:
	void log(QString text);
	
private:
	QStringList logs;
	bool logHasBeenWritten;
	
};

#endif // LOGGER_H