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
	//void closeEvent( QCloseEvent *event );

protected:
	void generateValue( QTreeWidgetItem *parent , const QString &name , QJsonValue &value , bool useParent = false );
	void generateChildren( QTreeWidgetItem *parent , QJsonObject &object );

public:
	QJsonObject toObject( void );
	inline bool isModified( void ) const { return contentModified; }

protected:
	bool contentModified;

signals:
	void modified( void );
	void unmodified( void );

public slots:
	void save( void );
	void showContextMenu( QPoint point );

	void addArrayItem( void );
	void removeArrayItem( void );

	void editTreeItem( QTreeWidgetItem *item , int column );
	void itemTextChanged( QTreeWidgetItem *item , int column );

	void setModified( void );
	void setUnmodified( void );
};

#endif // VJSONFORM_H
