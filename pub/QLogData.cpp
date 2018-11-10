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
  strftime(const_cast<char*>(str.data()), 32, "%FT%T", &local);
  return str;
}

QLogData::QLogData()
{

}

QLogData::QLogData(int32_t level)
{
	size = 0;
	calendar = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
	milsec = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now().time_since_epoch()).count() % 1000;
	process = QCoreApplication::applicationPid();
	thread = (uint64_t)QThread::currentThreadId();
	this->level = level;
}

QLogData::~QLogData()
{
}
