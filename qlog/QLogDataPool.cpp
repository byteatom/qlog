#include "QLogDataPool.h"

#include "GlobalData.h"

QLogData* allocQLogData()
{
	QLogData* data = dataAllocator.allocate(1);
	new (data) QLogData();
	return data;
}

QLogData* allocQLogData(int32_t level)
{
	QLogData* data = dataAllocator.allocate(1);
	new (data) QLogData(level);
	return data;
}

void freeQLogData(QLogData* data)
{
	data->~QLogData();
	dataAllocator.deallocate(data, 1);
}
