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

protected:
	VJsonForm* VTabWidgetArea::makeVJsonForm( void );
	VDraggableTabWidget* VTabWidgetArea::makeVDraggableTabWidget( void );
	VDraggableTabWidget* getFirstTabWidget( QSplitter *splitter ) const;

protected:
	QMap< QWidget* , VDraggableTabWidget* > tabMap;

public slots:
	void split( Qt::Orientation orientation );
	inline void splitHorizontally( void ) { split( Qt::Vertical ); }
	inline void splitVertically( void ) { split( Qt::Horizontal ); }
	void splitCollapse( void );

protected slots:
	void tabWidgetTabAdded( QWidget *widget );
	void tabWidgetTabDestroyed( QObject *object );
	void tabWidgetCountChanged( int count );
	void tabCloseRequested( int index );
	//void removeTabWidgetFromLayout( QObject *tabWidget ) { removeTabWidgetFromLayout( dynamic_cast< VDraggableTabWidget* >( tabWidget ) ); }
	void removeTabWidgetFromLayout( QObject *tabWidget ) { removeTabWidgetFromLayout( (VDraggableTabWidget*)tabWidget ); }
	void removeTabWidgetFromLayout( VDraggableTabWidget *tabWidget );
};

#endif // VTABWIDGETAREA_H
