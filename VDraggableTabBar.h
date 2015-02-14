#ifndef VDRAGGABLETABBAR_H
#define VDRAGGABLETABBAR_H

#include <iostream>

#include <QMouseEvent>
#include <QTabBar>
#include <QTabWidget>

class VDraggableTabBar : public QTabBar
{
public:
	VDraggableTabBar( void );
	~VDraggableTabBar();

protected slots:
	void dragEnterEvent( QDragEnterEvent *event );
	void dragMoveEvent( QDragMoveEvent *event );
	void dragLeaveEvent( QDragLeaveEvent *event );
};

class VDraggableTabWidget : public QTabWidget
{
public:
	VDraggableTabWidget( void );
	~VDraggableTabWidget();

protected slots:
	void dragEnterEvent( QDragEnterEvent *event );
	void dragMoveEvent( QDragMoveEvent *event );
	void dragLeaveEvent( QDragLeaveEvent *event );
};

#endif // VDRAGGABLETABBAR_H
