#ifndef VJSONFORM_H
#define VJSONFORM_H

#include "CJsonTemplate.h"
#include "qtil.h"

#include <QAction>
#include <QCloseEvent>
#include <QComboBox>
#include <QItemEditorCreatorBase>
#include <QItemEditorFactory>
#include <QMenu>
#include <QTreeWidget>
#include <QWidget>

class ColorListEditor : public QComboBox
{
	Q_OBJECT
	Q_PROPERTY(QColor color READ color WRITE setColor USER true)

public:
	ColorListEditor(QWidget *widget = 0);

public:
	QColor color() const;
	void setColor(QColor c);

private:
	void populateList();
};

class VStringListEdit : public QComboBox
{
	Q_OBJECT
	Q_PROPERTY( QString string READ getString WRITE setString USER true )
	Q_PROPERTY( QStringList stringList READ getStringList WRITE setStringList USER true )

public:
	VStringListEdit( QWidget *widget = NULL );

public:
	QString getString( void ) const;
	void setString( QString str );

	QStringList getStringList( void ) const;
	void setStringList( QStringList list );

private:
	void populateList( void );
};

class VJsonForm : public QTreeWidget
{
	Q_OBJECT

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
