#include "FileSink.h"

#include <functional>
#include <chrono>

#include <QDateTime>
#include <QDir>
#include <QDirIterator>
#include <QFile>
#include <QFileInfo>
#include <QTextStream>
#include <QCoreApplication>

#include "QLogDataPool.h"

const char* const QLogLevelStr[] = {
	"{NON}",
	"{EMG}",
	"{ALT}",
	"{CRT}",
	"{ERR}",
	"{WRN}",
	"{NTC}",
	"{INF}",
	"{DGB}",
	"{ALL}"
};

FileSink::FileSink()
{
}

FileSink::~FileSink()
{
	mutex.lock();
	exit = true;
	mutex.unlock();
	cond.notify_all();
	if (thread.joinable()) thread.join();
}

void FileSink::start()
{
	if (!thread.joinable())
		thread = std::thread{&FileSink::run, this};
}

void FileSink::push(QLogData* data)
{
	mutex.lock();
	if (queue.size() > 8192) {
		QLogData* tmpData = queue.front();
		queue.pop();
		freeQLogData(tmpData);
	}
	qint64 end = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now().time_since_epoch()).count() % 1000;
	if (end > data->milsec)
		data->delta = end - data->milsec;
	else if (end < data->milsec)
		data->delta = 1000 - data->milsec + end;
	else
		data->delta = 0;
	queue.push(data);
	mutex.unlock();
	cond.notify_all();
}

void FileSink::run()
{
	QString name{"log"};
	QDir dir;
	dir.mkdir(name);
	QDirIterator dirIt{name, QDir::Files, QDirIterator::Subdirectories};
	while (dirIt.hasNext()) {
		if (QFileInfo(dirIt.next()).lastModified().daysTo(QDateTime::currentDateTime()) > 30)
			dir.remove(dirIt.filePath());
	}

	name.append("/")
		.append(QFileInfo(QCoreApplication::applicationFilePath()).baseName())
		.append(QDateTime::currentDateTime().toString("-yyyyMMdd-HHmmss.log"));
	QFile file{name};
	if (!file.open(QIODevice::WriteOnly|QIODevice::Append)) return;
	QTextStream stream{&file};

	while (true) {
		std::unique_lock<std::mutex> lock(mutex);
		if (queue.empty())
			cond.wait(lock);
		QLogData *data = nullptr;
		if (queue.empty() && exit)
			break;
		else {
			data = queue.front();
			queue.pop();
		}
		lock.unlock();

		if (!data) continue;

		stream	<< QLogLevelStr[data->level]
				<< timet2str(data->calendar).c_str()
				<< ":"
				<< QString("%1").arg(data->milsec, 3, 10, QChar('0'))
				<< "> "
				<< data->text
				<< "\r\n";

		stream.flush();

		freeQLogData(data);
	}
}
