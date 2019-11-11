#include "QLogData.h"

#include <chrono>

#include <QCoreApplication>
#include <QThread>

std::string timet2str(const std::time_t& time)
{
  std::tm local;
#if (defined(WIN32) || defined(_WIN32) || defined(__WIN32__))
  localtime_s(&local, &time);
#else
  localtime_r(&time, &local); // POSIX
#endif
  std::string str;
  str.resize(32);
  size_t size = strftime(const_cast<char*>(str.data()), 32, "%FT%T", &local);
  str.resize(size);
  return str;
}

QLogData::QLogData()
{

}

QLogData::QLogData(int32_t level)
{
	size = 0;
	auto now = std::chrono::system_clock::now();
	calendar = std::chrono::system_clock::to_time_t(now);
	milsec = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count() % 1000;
	process = QCoreApplication::applicationPid();
	thread = (uint64_t)QThread::currentThreadId();
	this->level = level;
}

QLogData::~QLogData()
{
}
