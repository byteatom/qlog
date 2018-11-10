#include "NetSink.h"

#include <cassert>
#include <chrono>

#include <QDataStream>

#include "QLogDataPool.h"

NetSink::NetSink(const QString& ipaddr, quint16 port) :
	addr{ipaddr},
	port{port}
{
}

NetSink::~NetSink()
{
	requestInterruption();
	cond.wakeAll();
	wait();
	for (auto data: queue)
		freeQLogData(data);
}

void NetSink::start()
{
	if (!QThread::isRunning())
		QThread::start(QThread::LowPriority);
}

void NetSink::push(QLogData *data)
{
	mutex.lock();
	if (queue.size() > 8192) {
		freeQLogData(queue.dequeue());
	}
	queue.enqueue(data);
	qint64 end = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now().time_since_epoch()).count() % 1000;
	if (end > data->milsec)
		data->delta = end - data->milsec;
	else if (end < data->milsec)
		data->delta = 1000 - data->milsec + end;
	else
		data->delta = 0;
	cond.wakeAll();
	mutex.unlock();
}

void NetSink::run()
{
	QByteArray bytes;
	QDataStream stream{&bytes, QIODevice::WriteOnly};
	QTcpSocket socket;
	while (!isInterruptionRequested()) {
		if (socket.state() != socket.ConnectedState) {
			socket.connectToHost(addr, port);
			if (!socket.waitForConnected(3000)) {
				continue;
			}
		}

		mutex.lock();
		if (queue.empty())
			cond.wait(&mutex);
		QLogData *data = nullptr;
		if (!queue.empty())
			data = queue.dequeue();
		mutex.unlock();
		if (!data)  continue;

		bytes.clear();
		stream.device()->seek(0);
		stream	<< data->size
				<< data->calendar
				<< data->milsec
				<< data->delta
				<< data->process
				<< data->thread
				<< data->level
				<< data->text
				<< data->var;
		data->size = bytes.size() - sizeof(data->size);
		stream.device()->seek(0);
		stream << data->size;

		freeQLogData(data);

		socket.write(bytes);
		socket.flush();
	}
}
