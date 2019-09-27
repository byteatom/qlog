#pragma once

#include "../pub/QLogData.h"
#include "../lfpAlloc/Allocator.hpp"

struct QLogDataPool {
	static QLogData* alloc();
	static QLogData* alloc(int32_t level);
	static void free(QLogData* data);
private:
	typedef lfpAlloc::lfpAllocator<QLogData, sizeof(QLogData) * 2> Allocator;
	static Allocator allocator;
};
