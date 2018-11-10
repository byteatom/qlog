# QLog

A debug and log library for Qt.

### Features:
* Simple.
* A log library with Qt type(e.g. QPoint, QRect ...) support, especially QPixmap/QImage/QBitmap etc.
* Privided a TCP server can receive and display log info and view image facilitating debug QPaint/QPixmap etc.
![screenshot1](https://github.com/byteatom/qlog/blob/master/screenshot/screenshot1.png)
* Save log to local file.

### Usage:
``` cpp
#include "QLog.h"
qDbg << "hello world" << QPoint{0, 0} << QPixmap{"path.jpg"};
```

link the static library qlog.lib
