#include "NetSink.h"

#include <QDataStream>
#include <QTcpSocket>

#include "../pub/QLogData.h"

NetSink NetSink::singleton;

void NetSink::add(const std::shared_ptr<QLogData>& data)
{
	singleton.push(data);
}

void NetSink::push(const std::shared_ptr<QLogData>& data)
{
	if (stoped)
		return;

	if (!started) {
		started = true;
		thread = std::thread{&NetSink::run, this};
	}

	mutex.lock();
	if (queue.size() > 8192) {
		queue.pop();
	}
	queue.push(data);
	mutex.unlock();
	cond.notify_all();
}

void NetSink::run()
{
	QByteArray bytes;
	QDataStream stream{&bytes, QIODevice::WriteOnly};
	QTcpSocket socket;
	socket.connectToHost("127.0.0.1", 34567);
	if (!socket.waitForConnected(3000)) {
		stoped = true;
		return;
	}

	while (true) {
		std::unique_lock<std::mutex> lock(mutex);
		if (queue.empty())
			cond.wait(lock, [this]{return !queue.empty() || exit;});
		if (exit)
			break;

		auto data = queue.front();
		queue.pop();
		lock.unlock();

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

		socket.write(bytes);
		socket.flush();
	}
}

void NetSink::quit()
{
	singleton.stop();
}

void NetSink::stop()
{
	stoped = true;
	mutex.lock();
	exit = true;
	mutex.unlock();
	cond.notify_all();
	if (thread.joinable())
		thread.join();
}
