#include "QLogDataPool.h"

QLogDataPool::Allocator QLogDataPool::allocator;

QLogData* QLogDataPool::alloc()
{
	QLogData* data = allocator.allocate(1);
	new (data) QLogData();
	return data;
}

QLogData* QLogDataPool::alloc(int32_t level)
{
	QLogData* data = allocator.allocate(1);
	new (data) QLogData(level);
	return data;
}

void QLogDataPool::free(QLogData* data)
{
	data->~QLogData();
	allocator.deallocate(data, 1);
}
