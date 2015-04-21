#ifndef VTABWIDGETAREA_H
#define VTABWIDGETAREA_H

#include "VDraggableTabBar.h"
#include "VJsonForm.h"

#include <QMap>
#include <QSplitter>
#include <QWidget>

class EventModifiedChangeFilter : public QObject
{
	Q_OBJECT

public:
	EventModifiedChangeFilter( QObject *parent );

protected:
	bool eventFilter( QObject *obj , QEvent *event );

signals:
	void objModified( QObject *obj , bool isModified );
};

class VTabWidgetArea : public QSplitter
{
	Q_OBJECT

public:
	VTabWidgetArea( void );
	~VTabWidgetArea();

protected:
	bool event( QEvent *event );

public:
	void addWidgetToArea( QWidget *widget ) { addWidgetToArea( widget , getActiveTabWidget() ); }
	void addWidgetToArea( QWidget *widget , VDraggableTabWidget *tabWidget );
	void showWidget( QWidget *widget );

	inline QWidget* getActiveWidget( void ) { return getActiveTabWidget() ? activeTabWidget->currentWidget() : NULL; }
	VDraggableTabWidget* getActiveTabWidget( void );

protected:
	VDraggableTabWidget* makeVDraggableTabWidget( void );
	VDraggableTabWidget* getFirstTabWidget( QSplitter *splitter ) const;

protected:
	EventModifiedChangeFilter *modifiedChangeFilter;
	VDraggableTabWidget *activeTabWidget;
	QMap< QWidget* , VDraggableTabWidget* > tabMap;

signals:
	void widgetDeleted( QWidget *widget );

public slots:
	void split( Qt::Orientation orientation );
	inline void splitHorizontally( void ) { split( Qt::Vertical ); }
	inline void splitVertically( void ) { split( Qt::Horizontal ); }
	inline void splitCollapse( void ) { removeTabWidgetFromLayout( getActiveTabWidget() ); }

protected slots:
	void tabWidgetTabAdded( QWidget *widget );
	void tabWidgetTabDestroyed( QObject *object );
	void tabWidgetCountChanged( int count );
	void tabWidgetCurrentChanged( int index );

	//void tabCloseRequested( int index );
	// NOTE: dynamic_cast will fail if the pointer is invalid
	//void removeTabWidgetFromLayout( QObject *tabWidget ) { removeTabWidgetFromLayout( dynamic_cast< VDraggableTabWidget* >( tabWidget ) ); }
	void removeTabWidgetFromLayout( QObject *tabWidget ) { removeTabWidgetFromLayout( (VDraggableTabWidget*)tabWidget ); }
	void removeTabWidgetFromLayout( VDraggableTabWidget *tabWidget );

	void subWidgetTitleChanged( const QString &title );
	void subWidgetModifiedChange( QObject *obj , bool isModified );
};

#endif // VTABWIDGETAREA_H
