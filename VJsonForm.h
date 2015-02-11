#ifndef VJSONFORM_H
#define VJSONFORM_H

#include "CJsonTemplate.h"

#include <QTreeWidget>
#include <QWidget>

class VJsonForm : public QTreeWidget
{
	Q_OBJECT

public:
	VJsonForm( QWidget *parent = NULL );
	~VJsonForm();

protected:
	void generateValue( QTreeWidgetItem *parent , const QString &name , QJsonValue &value , bool useParent = false );
	void generateChildren( QTreeWidgetItem *parent , QJsonObject &object );

signals:

public slots:
};

#endif // VJSONFORM_H
