#include "DataView.h"
#include "ui_DataView.h"

#include <QBitmap>

#include "VarItem.h"

DataView::DataView(QWidget *parent) :
	QDialog(parent),
	table(nullptr),
	row(0),
	col(0),
	ui(new Ui::DataView)
{
	ui->setupUi(this);

	setAttribute(Qt::WA_DeleteOnClose);

	QBrush bgBrush(Qt::Dense7Pattern);
	scene.setBackgroundBrush(bgBrush);
	view = new QGraphicsView(&scene);
	view->setAlignment(Qt::AlignTop | Qt::AlignLeft);
	ui->stacked->addWidget(view);

	connect(ui->previous, &QPushButton::clicked, this, &DataView::previous);
	connect(ui->next, &QPushButton::clicked, this, &DataView::next);
}

DataView::~DataView()
{
	delete view;
	delete ui;
}

bool DataView::canShow(QTableWidgetItem *item)
{
	table = item->tableWidget();
	col = item->column();
	return load(item);
}

void DataView::previous()
{
	int preRow = row - 1;
	while (0 <= preRow) {
		if (load(table->item(preRow, col))) break;
		preRow -= 1;
	}
	if (preRow < 0) {
		ui->previous->setEnabled(false);
	}
	return;
}

void DataView::next()
{
	int nextRow = row + 1;
	while (table->rowCount() > nextRow) {
		if (load(table->item(nextRow, col))) break;
		nextRow += 1;
	}
	if (table->rowCount() <= nextRow) {
		ui->next->setEnabled(false);
	}
	return;
}

void DataView::addPixmap(const QPixmap& pixmap)
{
	scene.clear();
	scene.setSceneRect(pixmap.rect());
	scene.addPixmap(pixmap);
	scene.addRect(pixmap.rect());
}

bool DataView::load(QTableWidgetItem *item)
{
	if (!item) return false;
	QVariant var = static_cast<VarItem *>(item)->var;
	if (var.isNull()) return false;

	QString text = QString("[%1] ").arg(item->row() + 1);
	switch (var.type()) {
	case QMetaType::QPixmap: {
		QPixmap pixmap = var.value<QPixmap>();
		addPixmap(pixmap);
		text.append(QString("[%1x%2] ").
					arg(pixmap.width()).arg(pixmap.height()));
		break;
		}
	case QMetaType::QBitmap: {
		QBitmap bitmap = var.value<QBitmap>();
		addPixmap(bitmap);
		text.append(QString("[%1x%2] ").
					arg(bitmap.width()).arg(bitmap.height()));
		break;
		}
	case QMetaType::QImage: {
		QImage image = var.value<QImage>();
		QPixmap pixmap = QPixmap::fromImage(image);
		scene.clear();
		addPixmap(pixmap);
		text.append(QString("[%1x%2] ").
					arg(pixmap.width()).arg(pixmap.height()));
		break;
		}
	default:
		return false;
	}
	row = item->row();
	ui->previous->setEnabled(0 < row);
	ui->next->setEnabled(table->rowCount() - 1 > row);
	QTableWidgetItem *textItem = table->item(row, item->column() + 1);
	ui->text->setText(text.append(textItem->text()));
	return true;
}
