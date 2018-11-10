#pragma once

#include <queue>
#include <mutex>
#include <condition_variable>
#include <thread>

struct QLogData;

class FileSink
{
public:
	FileSink();
	~FileSink();

	void start();
	void push(QLogData* data);

private:
	void run();

	std::queue<QLogData*> queue;
	std::mutex mutex;
	std::condition_variable cond;
	std::thread thread;
	bool exit = false;
};
