#pragma once

#include <QDialog>
#include <QTcpServer>
#include <QDataStream>
#include <QList>

#include "../pub/QLogData.h"
#include "VarItem.h"

namespace Ui {
class MainWnd;
}

class MainWnd : public QDialog
{
	Q_OBJECT

public:
	enum {
		COL_PROCESS,
		COL_THREAD,
		COL_STAMP,
		COL_LEVEL,
		COL_DATA,
		COL_TEXT
	};

	explicit MainWnd(QWidget *parent = 0);
	~MainWnd();

private:
	void onOff(bool checked);
	void accept();
	void read();
	void disconnect();
	void addLog();
	void clear();
	void bottom();
	void top();
	void filterChanged(const QString &text);
	void find();
	void showData(QTableWidgetItem *item);

	QTcpServer server;
	QLogData log;
	QDataStream stream;
	QList<QTableWidgetItem *> finded;
	int currentFinded;

	Ui::MainWnd *ui;
};
