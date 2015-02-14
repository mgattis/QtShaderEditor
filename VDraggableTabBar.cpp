#include "VDraggableTabBar.h"

VDraggableTabBar::VDraggableTabBar()
{
	//
}

VDraggableTabBar::~VDraggableTabBar()
{
	//
}

void VDraggableTabBar::dragEnterEvent( QDragEnterEvent *event )
{
	if( event )
	{
		std::cout << "enter" << std::endl;
		event->accept();
	}
}

void VDraggableTabBar::dragMoveEvent( QDragMoveEvent *event )
{
	if( event )
	{
		std::cout << "move" << std::endl;
		event->accept();
	}
}

void VDraggableTabBar::dragLeaveEvent( QDragLeaveEvent *event )
{
	if( event )
	{
		std::cout << "leave" << std::endl;
		event->accept();
	}
}

VDraggableTabWidget::VDraggableTabWidget( void )
{
	VDraggableTabBar *tabBar = new VDraggableTabBar();
	this->setTabBar( tabBar );
}

VDraggableTabWidget::~VDraggableTabWidget()
{
	//
}

void VDraggableTabWidget::dragEnterEvent( QDragEnterEvent *event )
{
	if( event )
	{
		std::cout << "enter" << std::endl;
		event->accept();
	}
}

void VDraggableTabWidget::dragMoveEvent( QDragMoveEvent *event )
{
	if( event )
	{
		std::cout << "move" << std::endl;
		event->accept();
	}
}

void VDraggableTabWidget::dragLeaveEvent( QDragLeaveEvent *event )
{
	if( event )
	{
		std::cout << "leave" << std::endl;
		event->accept();
	}
}
