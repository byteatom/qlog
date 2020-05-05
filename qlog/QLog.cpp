#include "../inc/QLog.h"

#include "../pub/QLogData.h"
#include "FileSink.h"
#include "NetSink.h"

QLog::QLog(Level level)
{
	data = std::make_shared<QLogData>(level);
	setString(&data->text, QIODevice::WriteOnly);
}

QLog::~QLog()
{
	setString(nullptr);

	qint64 end = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count() % 1000;
	if (end > data->milsec)
		data->delta = end - data->milsec;
	else if (end < data->milsec)
		data->delta = 1000 - data->milsec + end;
	else
		data->delta = 0;

	FileSink::add(data);

	NetSink::add(data);
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

void QLog::quit() {
	FileSink::quit();
	NetSink::quit();
}
