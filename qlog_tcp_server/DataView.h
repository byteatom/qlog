#pragma once

#include <QDialog>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QTableWidgetItem>

namespace Ui {
class DataView;
}

class DataView : public QDialog
{
	Q_OBJECT

public:
	explicit DataView(QWidget *parent);
	~DataView();
	bool canShow(QTableWidgetItem *item);

private:
	void previous();
	void next();
	void addPixmap(const QPixmap& pixmap);
	bool load(QTableWidgetItem *item);
	QGraphicsScene scene;
	QGraphicsView *view;
	QTableWidget *table;
	int row;
	int col;

	Ui::DataView *ui;
};
