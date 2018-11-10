#pragma once

#include <QThread>
#include <QString>
#include <QTcpSocket>
#include <QQueue>
#include <QMutex>
#include <QWaitCondition>

struct QLogData;

class NetSink : public QThread
{
public:
	NetSink(const QString& addr = "127.0.0.1", quint16 port = 34567);
	virtual ~NetSink();

	void start();
	void push(QLogData *log);

protected:
	virtual void run() override;

private:
	QString addr;
	quint16 port;
	QQueue<QLogData *> queue;
	QMutex mutex;
	QWaitCondition cond;
};
