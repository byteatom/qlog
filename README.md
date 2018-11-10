# QLog

A debug and log library for Qt.

# Features:
* Simple.
* A log library
* Privided a TCP server can receive and display log.
* Save log to local file
* Qt type(e.g. QPoint ...) support.
* QPixmap etc. can be displayed in the server UI facilitate debug QPaint etc.

# Usage:
``` cpp
#include "QLog.h"
qlog << "hello world" << QPoint{0, 0} << QPixmap{"path.jpg"} << qDbg;
```

link the static library qlog.lib
