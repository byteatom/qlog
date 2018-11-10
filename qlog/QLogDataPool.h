#pragma once

#include "../pub/QLogData.h"

QLogData* allocQLogData();
QLogData* allocQLogData(int32_t level);
void freeQLogData(QLogData* data);
