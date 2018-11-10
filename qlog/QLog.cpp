#include "../inc/QLog.h"

#include "../pub/QLogData.h"
#include "FileSink.h"
#include "NetSink.h"
#include "QLogDataPool.h"
#include "GlobalData.h"

QLog::QLog(Level level)
{
	data = allocQLogData(level);
	setString(&data->text, QIODevice::WriteOnly);
}

QLog::~QLog()
{
	fileSink.start();
	fileSink.push(data);

	netSink.start();
	QLogData* netData = allocQLogData();
	*netData = *data;
	netSink.push(netData);
}

QLog& QLog::operator<<(const char* string)
{
	operator<<(QString::fromUtf8(string));
	return *this;
}

QLog& QLog::operator<<(const std::string &string)
{
	operator<<(QString::fromStdString(string));
	return *this;
}

QLog& QLog::operator<<(const wchar_t* string)
{
	operator<<(QString::fromWCharArray(string));
	return *this;
}

QLog& QLog::operator<<(const std::wstring &string)
{
	operator<<(QString::fromStdWString(string));
	return *this;
}

QLog& QLog::operator<<(const QPoint &point)
{
	operator<<("(") << point.x() << "," << point.y() << ")";
	return *this;
}

QLog& QLog::operator<<(const QPointF &point)
{
	operator<<("(") << point.x() << "," << point.y() << ")";
	return *this;
}

QLog& QLog::operator<<(const QSize &size)
{
	operator<<("(") << size.width() << "," << size.height() << ")";
	return *this;
}

QLog& QLog::operator<<(const QSizeF &size)
{
	operator<<("(") << size.width() << "," << size.height() << ")";
	return *this;
}

QLog& QLog::operator<<(const QRect &rect)
{
	operator<<("(") << rect.x() << "," << rect.y() << "," << rect.width() << "," << rect.height() << ")";
	return *this;
}

QLog& QLog::operator<<(const QRectF &rect)
{
	operator<<("(") << rect.x() << "," << rect.y() << "," << rect.width() << "," << rect.height() << ")";
	return *this;
}

QLog& QLog::operator<<(const QPixmap &pixmap)
{
	data->var = pixmap;
	return *this;
}

QLog& QLog::operator<<(const QBitmap &bitmap)
{
	data->var = bitmap;
	return *this;
}

QLog& QLog::operator<<(const QImage &image)
{
	data->var = image;
	return *this;
}
