#ifndef LOGGER_H
#define LOGGER_H

#include <QObject>
#include <QList>
#include <QString>

class Logger : public QObject
{
	Q_OBJECT
public:
	explicit Logger(QObject *parent = 0);
	
signals:
	void logUpdated(QString text);
	
public slots:
	void log(QString text);
	
private:
	QList<QString> logs;
	
};

#endif // LOGGER_H
