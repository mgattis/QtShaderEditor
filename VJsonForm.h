#ifndef VJSONFORM_H
#define VJSONFORM_H

#include "CJsonTemplate.h"
#include "qtil.h"

#include <QAction>
#include <QApplication>
#include <QCloseEvent>
#include <QComboBox>
#include <QItemDelegate>
#include <QItemEditorCreatorBase>
#include <QItemEditorFactory>
#include <QMenu>
#include <QModelIndex>
#include <QStandardItemModel>
#include <QStringList>
#include <QTreeWidget>
#include <QVariant>
#include <QWidget>

class ComboBoxDelegate : public QItemDelegate
{
	Q_OBJECT

public:
	ComboBoxDelegate( QObject *parent = NULL );

public:
	QWidget* createEditor( QWidget *parent , const QStyleOptionViewItem &option , const QModelIndex &index ) const;
	void setEditorData( QWidget *editor , const QModelIndex &index ) const;
	void setModelData( QWidget *editor , QAbstractItemModel *model , const QModelIndex &index ) const;
	void updateEditorGeometry( QWidget *editor , const QStyleOptionViewItem &option , const QModelIndex &index ) const;
	void paint( QPainter *painter , const QStyleOptionViewItem &option , const QModelIndex &index ) const;
};

class VJsonForm : public QTreeWidget
{
	Q_OBJECT

	friend class ComboBoxDelegate;

public:
	VJsonForm( QWidget *parent = NULL );
	~VJsonForm();

protected:
	void closeEvent( QCloseEvent *event );

public:
	QJsonObject toObject( void );

protected:
	void generateValue( QTreeWidgetItem *parent , const QString &name , QJsonValue &value , bool useParent = false );
	void generateChildren( QTreeWidgetItem *parent , QJsonObject &object );

protected:
	ComboBoxDelegate *delegate;

public slots:
	void save( void );
	void showContextMenu( QPoint point );

	void addArrayItem( void );
	void removeArrayItem( void );

	void editTreeItem( QTreeWidgetItem *item , int column );
	void itemTextChanged( QTreeWidgetItem *item , int column );

	inline setModified( void ) { setModified( true ); }
	inline setUnmodified( void ) { setModified( false ); }
	void setModified( bool modified ) { if( modified != this->isWindowModified() ) this->setWindowModified( modified ); }
};

#endif // VJSONFORM_H
