#pragma once

#include <cstdint>
#include <ctime>
#include <string>

#include <QVariant>

std::string timet2str(const std::time_t& time);

struct QLogData
{
	QLogData();
	QLogData(int32_t level);
	~QLogData();
	size_t size;
	time_t calendar;
	int32_t milsec;
	int32_t delta;
	qint64 process;
	uint64_t thread;
	int32_t level;
	QString text;
	QVariant var;
};
