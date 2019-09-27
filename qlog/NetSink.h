#pragma once

#include <queue>
#include <mutex>
#include <condition_variable>
#include <thread>

#include <QString>

struct QLogData;

class NetSink
{
public:
	static void add(QLogData* data);
	static void quit();

private:
	void push(QLogData* data);
	void run();
	void stop();

	std::queue<QLogData*> queue;
	std::mutex mutex;
	std::condition_variable cond;
	std::thread thread;
	bool started = false;
	bool stoped = false;
	bool exit = false;

	static NetSink singleton;
};
