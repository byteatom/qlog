#include "VarItem.h"

VarItem::VarItem(QVariant &var) :    
	QTableWidgetItem(QString(var.typeName())),
	var(var)
{

}

VarItem::~VarItem()
{

}

