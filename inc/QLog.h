#pragma once

#include <string>
#include <memory>

#include <QTextStream>
#include <QPoint>
#include <QSize>
#include <QRect>
#include <QPixmap>
#include <QBitmap>
#include <QImage>

#if defined QLOG_LIB
#define QLOG_API Q_DECL_EXPORT
#else
#define QLOG_API Q_DECL_IMPORT
#pragma comment(lib, "qlog.lib")
#endif

struct QLogData;

class QLOG_API QLog : public QTextStream
{
public:
	enum Level{
		None,
		Emergency,
		Alert,
		Critical,
		Error,
		Warnning,
		Notice,
		Infomation,
		Debug,
		ALL,
	};

	QLog(Level level);
	~QLog();

	using QTextStream::operator<<;	//must stand before overrides below

	QLog& operator<<(const char* string);
	QLog& operator<<(const std::string &string);
	QLog& operator<<(const wchar_t* string);
	QLog& operator<<(const std::wstring &string);
	QLog& operator<<(const QPoint &point);
	QLog& operator<<(const QPointF &point);
	QLog& operator<<(const QSize &size);
	QLog& operator<<(const QSizeF &size);
	QLog& operator<<(const QRect &rect);
	QLog& operator<<(const QRectF &rect);
	QLog& operator<<(const QPixmap &pixmap);
	QLog& operator<<(const QBitmap &bitmap);
	QLog& operator<<(const QImage &image);

	static void quit();

private:
	std::shared_ptr<QLogData> data;
};

template<std::size_t Len>
constexpr const char* staticStripDirImpl(const char(&path)[Len], std::size_t pos)
{
	return pos == 0
			? path
			: (path[pos] == '/' || path[pos] == '\\')
			  ? path + pos + 1
			  : staticStripDirImpl(path, --pos);
}

template<std::size_t Len>
constexpr const char* staticStripDir(const char(&str)[Len])
{
	return staticStripDirImpl(str, Len - 1);
}

#define STRINGIFY(x) #x
#define STRINGIZE(x) STRINGIFY(x)

#define __FILE_LINE__ staticStripDir(__FILE__ ":" STRINGIZE(__LINE__) "> ")

#define QLOG_CREATE(level) QLog(level)<<__FILE_LINE__

#define qEmerg QLOG_CREATE(QLog::Emergency)
#define qAlert QLOG_CREATE(QLog::Alert)
#define qCriti QLOG_CREATE(QLog::Critical)
#define qErr QLOG_CREATE(QLog::Error)
#define qWarn QLOG_CREATE(QLog::Warnning)
#define qNotice QLOG_CREATE(QLog::Notice)
#define qInf QLOG_CREATE(QLog::Infomation)
#define qDbg QLOG_CREATE(QLog::Debug)
