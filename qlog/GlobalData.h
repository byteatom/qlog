#pragma once

#include "../pub/QLogData.h"
#include "../lfpAlloc/Allocator.hpp"
#include "FileSink.h"
#include "NetSink.h"

typedef lfpAlloc::lfpAllocator<QLogData, sizeof(QLogData) * 2> QLogDataAllocator;
extern QLogDataAllocator dataAllocator;
extern FileSink fileSink;
extern NetSink netSink;
