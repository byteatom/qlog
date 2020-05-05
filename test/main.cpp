#include <QCoreApplication>

#include "../inc/QLog.h"

int main(int argc, char *argv[])
{
	QCoreApplication a(argc, argv);

	for (int i = 0; i < 1000000; ++i) {
		qDbg<<"hello";
	}

	//a.exec();

	QLog::quit();

	return 0;
}
