#ifndef VDRAGGABLETABBAR_H
#define VDRAGGABLETABBAR_H

#include <iostream>

#include <QApplication>
#include <QDrag>
#include <QMimeData>
#include <QMouseEvent>
#include <QTabBar>
#include <QTabWidget>

class VDraggableTabWidget;

class VDraggableTabBar : public QTabBar
{
	Q_OBJECT

public:
	VDraggableTabBar( VDraggableTabWidget *parent );
	~VDraggableTabBar();

protected:
	VDraggableTabWidget *parentTabWidget;
	QPoint dragStartPos;

signals:
	void tabMoveRequested( int fromIndex , int toIndex );

protected slots:
#if 1
	void mousePressEvent( QMouseEvent *event );
	void mouseMoveEvent( QMouseEvent *event );
	void mouseReleaseEvent( QMouseEvent *event );
#endif

	void dragEnterEvent( QDragEnterEvent *event );
	void dragMoveEvent( QDragMoveEvent *event );
	void dragLeaveEvent( QDragLeaveEvent *event );

	void dropEvent( QDropEvent *event );
};

class VDraggableTabWidget : public QTabWidget
{
	Q_OBJECT

public:
	VDraggableTabWidget( void );
	~VDraggableTabWidget();

protected:
	void tabInserted( int index );
	void tabRemoved( int index );

signals:
	void widgetAdded( QWidget *widget );
	void widgetRemoved( QWidget *widget );
	void tabCountChanged( int count );

protected slots:
	void dragEnterEvent( QDragEnterEvent *event );
	void dragMoveEvent( QDragMoveEvent *event );
	void dragLeaveEvent( QDragLeaveEvent *event );

	void dropEvent( QDropEvent *event );
};

#endif // VDRAGGABLETABBAR_H
