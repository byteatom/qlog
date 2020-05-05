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
	static void add(const std::shared_ptr<QLogData>& data);
	static void quit();

private:
	void push(const std::shared_ptr<QLogData>& data);
	void run();
	void stop();

	std::queue<std::shared_ptr<QLogData>> queue;
	std::mutex mutex;
	std::condition_variable cond;
	std::thread thread;
	bool started = false;
	bool stoped = false;
	bool exit = false;

	static NetSink singleton;
};
