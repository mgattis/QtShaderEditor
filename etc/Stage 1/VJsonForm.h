#ifndef VJSONFORM_H
#define VJSONFORM_H

#include "CJsonTemplate.h"
#include "qtil.h"

#include <QAction>
#include <QMenu>
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

public slots:
	void showContextMenu( QPoint point );

	void addArrayItem( void );
	void removeArrayItem( void );
};

#endif // VJSONFORM_H
