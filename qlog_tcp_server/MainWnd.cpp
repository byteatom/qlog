#include "MainWnd.h"
#include "ui_MainWnd.h"

#include <QTcpSocket>
#include <QVariant>
#include <QMessageBox>

#include "DataView.h"

MainWnd::MainWnd(QWidget *parent) :
	QDialog(parent),
	currentFinded(0),
	ui(new Ui::MainWnd)
{
	ui->setupUi(this);

	//ui->table->setStyleSheet("QHeaderView::section { background-color:lightgray }");

	ui->table->setColumnWidth(COL_PROCESS, 60);
	ui->table->setColumnWidth(COL_THREAD, 60);
	ui->table->setColumnWidth(COL_STAMP, 170);
	ui->table->setColumnWidth(COL_LEVEL, 50);
	ui->table->horizontalHeaderItem(COL_TEXT)->setTextAlignment(Qt::AlignLeft);

	ui->filter->setFocus();

	log.size = 0;

	connect(ui->onOff, &QPushButton::toggled, this, &MainWnd::onOff);
	connect(ui->clear, &QPushButton::clicked, this, &MainWnd::clear);
	connect(ui->bottom, &QPushButton::clicked, this, &MainWnd::bottom);
	connect(ui->top, &QPushButton::clicked, this, &MainWnd::top);
	connect(ui->filter, &QLineEdit::textChanged, this, &MainWnd::filterChanged);
	connect(ui->find, &QPushButton::clicked, this, &MainWnd::find);
	connect(ui->table, &QTableWidget::itemDoubleClicked, this, &MainWnd::showData);
	connect(&server, &QTcpServer::newConnection, this, &MainWnd::accept);

	ui->onOff->setChecked(true);
}

MainWnd::~MainWnd()
{
	delete ui;
}

void MainWnd::onOff(bool checked)
{
	if (checked) {
		bool ok = server.listen(QHostAddress::Any, ui->port->text().toUShort());
		if (!ok) {
			QMessageBox::critical(this, tr("QLog Server"),
				QString(tr("Port %1 is in use, try another and start again.")));
			ui->onOff->setChecked(false);
		}
	} else {
		server.close();
	}
	ui->onOff->setText(checked ? tr("Stop") : tr("Start"));
}

void MainWnd::accept()
{
	QTcpSocket *client = server.nextPendingConnection();
	if (client) {
		connect(client, &QTcpSocket::readyRead, this, &MainWnd::read);
	}
}

void MainWnd::read()
{
	QTcpSocket *client = static_cast<QTcpSocket *>(sender());
	if (!client) return;

	while (client->bytesAvailable() > sizeof(log.size)) {
		stream.setDevice(client);
		if (0 == log.size) {
			stream >> log.size;
		}
		if (0 == log.size)
			continue;
		if (client->bytesAvailable() < log.size)
			break;

		stream	>> log.calendar
				>> log.milsec
				>> log.delta
				>> log.process
				>> log.thread
				>> log.level
				>> log.text
				>> log.var;
		log.size = 0;

		addLog();
	}
}

void MainWnd::addLog()
{
	int row = ui->table->rowCount();
	ui->table->insertRow(row);
	QTableWidgetItem *item;

	item= new QTableWidgetItem(QString::number(log.process));
	ui->table->setItem(row, COL_PROCESS, item);

	item = new QTableWidgetItem(QString::number(log.thread));
	ui->table->setItem(row, COL_THREAD, item);

	QString stamp = QString::fromStdString(timet2str(log.calendar));
	stamp.append(":").append(QString::number(log.milsec));
	if (log.delta > 0)
		stamp.append("+").append(QString::number(log.delta));
	item = new QTableWidgetItem(stamp);
	ui->table->setItem(row, COL_STAMP, item);

	item = new QTableWidgetItem(QString::number(log.level));
	ui->table->setItem(row, COL_LEVEL, item);

	item = new VarItem(log.var);
	ui->table->setItem(row, COL_DATA, item);

	item = new QTableWidgetItem(log.text);
	ui->table->setItem(row, COL_TEXT, item);

	if (ui->scroll->isChecked())
		ui->table->scrollToBottom();
}

void MainWnd::clear()
{
	ui->table->setRowCount(0);
	finded.clear();
	currentFinded = 0;
}

void MainWnd::bottom()
{
	ui->table->scrollToBottom();
}

void MainWnd::top()
{
	ui->table->scrollToTop();
}

void MainWnd::filterChanged(const QString &text)
{
	finded.clear();
	currentFinded = 0;
}

void MainWnd::find()
{
	if (finded.empty()) {
		finded = ui->table->findItems(ui->filter->text(), Qt::MatchContains);
		currentFinded = 0;
	}

	if (finded.empty()) return;

	++currentFinded;
	if (currentFinded == finded.size()) currentFinded = 0;

	ui->table->setCurrentItem(finded[currentFinded]);
}

void MainWnd::showData(QTableWidgetItem *item)
{
	if (MainWnd::COL_DATA != item->column()) return;

	DataView *view = new DataView(this);
	if (view->canShow(item))
		view->show();
	else
		delete view;
}
