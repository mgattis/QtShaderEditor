#ifndef VJSONFORM_H
#define VJSONFORM_H

#include "CJsonTemplate.h"
#include "log.h"
#include "qtil.h"

#include <QAction>
#include <QApplication>
#include <QCloseEvent>
#include <QComboBox>
#include <QFileDialog>
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
};

class VJsonForm : public QTreeWidget
{
	Q_OBJECT

	// For itemFromIndex( const QModelIndex );
	friend class ComboBoxDelegate;

public:
	VJsonForm( QWidget *parent = NULL );
	~VJsonForm();

protected:
	void closeEvent( QCloseEvent *event );

public:
	void load( const QString &path );
	void save( void );

	QJsonObject toObject( void );

protected:
	//void generateValue( QTreeWidgetItem *parent , const QString &name , QJsonValue &value , bool useParent = false );
	//void generateChildren( QTreeWidgetItem *parent , QJsonObject &object );

protected:
	QString filePath;
	ComboBoxDelegate *delegate;
	VJsonFormItem *lastContextItem;

public slots:
	void showContextMenu( QPoint point );

	void addArrayItem( void );
	void removeArrayItem( void );
	void toggleStructureItem( void );

	void editTreeItem( QTreeWidgetItem *item , int column );
	void itemTextChanged( QTreeWidgetItem *item , int column );

	inline void setModified( void ) { setModified( true ); }
	inline void setUnmodified( void ) { setModified( false ); }
	void setModified( bool modified ) { if( modified != this->isWindowModified() ) this->setWindowModified( modified ); }
};

#endif // VJSONFORM_H
