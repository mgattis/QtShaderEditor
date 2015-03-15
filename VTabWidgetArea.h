#ifndef VTABWIDGETAREA_H
#define VTABWIDGETAREA_H

#include "VDraggableTabBar.h"
#include "VJsonForm.h"

#include <QMap>
#include <QSplitter>
#include <QWidget>

class VTabWidgetArea : public QSplitter
{
	Q_OBJECT

public:
	VTabWidgetArea();
	~VTabWidgetArea();

public:
	void addWidgetToArea( QWidget *widget , const QString &title ) { addWidgetToArea( widget , title , getActiveTabWidget() ); }
	void addWidgetToArea( QWidget *widget , const QString &title , VDraggableTabWidget *tabWidget ) { tabWidget->addTab( widget , title ); }
	VDraggableTabWidget* getActiveTabWidget( void );

protected:
	VJsonForm* VTabWidgetArea::makeVJsonForm( void );
	VDraggableTabWidget* VTabWidgetArea::makeVDraggableTabWidget( void );
	VDraggableTabWidget* getFirstTabWidget( QSplitter *splitter ) const;

protected:
	VDraggableTabWidget *activeTabWidget;
	//QWidget *activeWidget;
	QMap< QWidget* , VDraggableTabWidget* > tabMap;

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
};

#endif // VTABWIDGETAREA_H
