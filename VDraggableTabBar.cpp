#include "VDraggableTabBar.h"

VDraggableTabBar::VDraggableTabBar( VDraggableTabWidget *parent )
{
	this->parentTabWidget = parent;

	//this->setAcceptDrops( true );
	this->setMovable( true );
	this->setTabsClosable( true );
}

VDraggableTabBar::~VDraggableTabBar()
{
	// Nothing to do
}

#if 1
void VDraggableTabBar::mousePressEvent( QMouseEvent *event )
{
	if( event )
	{
		if( event->button() == Qt::LeftButton )
		{
			//std::cout << "bar press" << std::endl;

			dragStartPos = event->pos();
			QTabBar::mousePressEvent( event );
		}
	}
}

void VDraggableTabBar::mouseMoveEvent( QMouseEvent *event )
{
	if( event )
	{
		if( event->buttons() & Qt::LeftButton && ( event->pos() - dragStartPos ).manhattanLength() >= QApplication::startDragDistance() )
		{
			//std::cout << "move" << std::endl;

			if( !this->geometry().contains( event->pos() , true ) )
			{
				parentTabWidget->setAcceptDrops( false );

				QDrag *drag = new QDrag( this );
				QMimeData *mimeData = new QMimeData;

				//QTabWidget *tabWidget = dynamic_cast< QTabWidget* >( this->parentWidget() );
				//mimeData->setProperty( "vjsonform" , QVariant( (void*)tabWidget->currentWidget() ) );
				mimeData->setData( "internal" , QByteArray( "testdata" ) );
				drag->setMimeData( mimeData );

				Qt::DropAction dropAction = drag->exec( Qt::MoveAction );

				if( dropAction )
				{
					std::cout << "dropAction" << std::endl;
					VDraggableTabWidget *target = dynamic_cast< VDraggableTabWidget* >( drag->target() );

					if( target )
						target->addTab( parentTabWidget->currentWidget() , parentTabWidget->tabText( parentTabWidget->currentIndex() ) );
				}
				else
				{
					std::cout << "!dropAction" << std::endl;
					int currentIndex = parentTabWidget->currentIndex();
					parentTabWidget->setCurrentIndex( -1 );
					parentTabWidget->setCurrentIndex( currentIndex );
				}

				parentTabWidget->setAcceptDrops( true );
			}
			else
				QTabBar::mouseMoveEvent( event );
		}
	}
}

void VDraggableTabBar::mouseReleaseEvent( QMouseEvent *event )
{
	if( event )
	{
		//std::cout << "bar release" << std::endl;

		QTabBar::mouseReleaseEvent( event );
	}
}
#endif

void VDraggableTabBar::dragEnterEvent( QDragEnterEvent *event )
{
	if( event )
	{
		std::cout << "bar enter" << std::endl;
		//event->ignore();
	}
}

void VDraggableTabBar::dragMoveEvent( QDragMoveEvent *event )
{
	if( event )
	{
		//std::cout << "bar move" << std::endl;
		//event->ignore();
	}
}

void VDraggableTabBar::dragLeaveEvent( QDragLeaveEvent *event )
{
	if( event )
	{
		std::cout << "bar leave" << std::endl;
		//event->accept();
	}
}

void VDraggableTabBar::dropEvent( QDropEvent *event )
{
	event->ignore();
}

//
// VDraggableTabWidget
//

VDraggableTabWidget::VDraggableTabWidget( void )
{
	this->setAcceptDrops( true );

	VDraggableTabBar *tabBar = new VDraggableTabBar( this );
	tabBar->setSizePolicy( QSizePolicy::Ignored , tabBar->sizePolicy().verticalPolicy() );
	this->setTabBar( tabBar );
}

VDraggableTabWidget::~VDraggableTabWidget()
{
	// Nothing to do
}

void VDraggableTabWidget::tabInserted( int index )
{
	this->setMovable( this->count() > 1 );
	QTabWidget::tabInserted( index );

	std::cout << QString( "%1 \"%2\" inserted into %3" ).arg( (int)this->widget( index ) ).arg( this->tabText( index ).toLatin1().data() ).arg( (int)this ).toLatin1().data() << std::endl;
	emit widgetAdded( this->widget( index ) );
	emit tabCountChanged( this->count() );
}

void VDraggableTabWidget::tabRemoved( int index )
{
	this->setMovable( this->count() > 1 );
	QTabWidget::tabRemoved( index );

	std::cout << QString( "window removed from %1" ).arg( (int)this ).toLatin1().data() << std::endl;
	//emit widgetRemoved( this->widget( index ) );

	emit tabCountChanged( this->count() );

	//if( !this->count() )
		//emit destructionRequested();
}

void VDraggableTabWidget::dragEnterEvent( QDragEnterEvent *event )
{
	if( event )
	{
		std::cout << "widget enter" << std::endl;

		if( event->source() != this )
		{
			QByteArray data = event->mimeData()->data( "internal" );

			if( data == "testdata" )
				event->accept();
		}
		else
			event->ignore();
	}
}

void VDraggableTabWidget::dragMoveEvent( QDragMoveEvent *event )
{
	if( event )
	{
		//std::cout << "widget move" << std::endl;
		//event->accept();
	}
}

void VDraggableTabWidget::dragLeaveEvent( QDragLeaveEvent *event )
{
	if( event )
	{
		std::cout << "widget leave" << std::endl;
		//event->ignore();
	}
}

void VDraggableTabWidget::dropEvent( QDropEvent *event )
{
	QByteArray data = event->mimeData()->data( "internal" );

	if( data == "testdata" )
		event->accept();
	else
		event->ignore();
}

void VDraggableTabWidget::setTabModified( int index )
{
	if( index >= 0 )
	{
		QString title = this->tabText( index );

		if( !title.endsWith( '*' , Qt::CaseInsensitive ) )
		{
			title.append( '*' );

			this->setTabText( index , title );
		}
	}
}

void VDraggableTabWidget::setTabUnmodified( int index )
{
	if( index >= 0 )
	{
		QString title = this->tabText( index );

		if( !title.endsWith( '*' , Qt::CaseInsensitive ) )
		{
			title.chop( 1 );

			this->setTabText( index , title );
		}
	}
}
