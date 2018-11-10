#pragma once

#include <QTableWidgetItem>
#include <QVariant>

class VarItem : public QTableWidgetItem
{
public:
    explicit VarItem(QVariant &var);
    virtual ~VarItem();

    QVariant var;
};
