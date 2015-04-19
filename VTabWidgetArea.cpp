#include "VTabWidgetArea.h"

EventModifiedChangeFilter::EventModifiedChangeFilter( QObject *parent ) : QObject( parent )
{
	// Nothing to do
}

bool EventModifiedChangeFilter::eventFilter( QObject *obj , QEvent *event )
{
	if( event->type() == QEvent::ModifiedChange )
		emit objModified( obj , ( (QWidget*)obj )->testAttribute( Qt::WA_WindowModified ) );

	// Don't eat the event
	return QObject::eventFilter( obj , event );
}

VTabWidgetArea::VTabWidgetArea( void )
{
	modifiedChangeFilter = new EventModifiedChangeFilter( this );
	connect( modifiedChangeFilter , SIGNAL(objModified(QObject*,bool)) , this , SLOT(subWidgetModifiedChange(QObject*,bool)) );

	VDraggableTabWidget *initialTab = makeVDraggableTabWidget();
	activeTabWidget = initialTab;

	this->addWidget( initialTab );
}

VTabWidgetArea::~VTabWidgetArea()
{
	// Nothing to do
}

bool VTabWidgetArea::event( QEvent *event )
{
	if( event && event->type() == QEvent::ModifiedChange )
		std::cout << "title modified" << std::endl;

	return QSplitter::event( event );
}

void VTabWidgetArea::addWidgetToArea( QWidget *widget , VDraggableTabWidget *tabWidget )
{
	widget->setAttribute( Qt::WA_DeleteOnClose );
	tabWidget->setCurrentIndex( tabWidget->addTab( widget , widget->windowTitle().replace( "[*]" , widget->isWindowModified() ? "*" : "" ) ) );
	widget->installEventFilter( modifiedChangeFilter );

	connect( widget , SIGNAL(windowTitleChanged(QString)) , this , SLOT(subWidgetTitleChanged(QString)) );
	connect( widget , SIGNAL(destroyed(QObject*)) , this , SLOT(tabWidgetTabDestroyed(QObject*)) );
}

void VTabWidgetArea::showWidget( QWidget *widget )
{
	VDraggableTabWidget *tabWidget = tabMap.value( widget , NULL );

	if( tabWidget )
		tabWidget->setCurrentWidget( widget );
}

VDraggableTabWidget* VTabWidgetArea::getActiveTabWidget( void )
{
	if( !activeTabWidget )
	{
		activeTabWidget = tabMap.value( this->focusWidget() , NULL );

		if( !activeTabWidget )
		{
			activeTabWidget = dynamic_cast< VDraggableTabWidget* >( this->focusWidget() );

			if( !activeTabWidget )
				activeTabWidget = getFirstTabWidget( this ); // This should never return NULL
		}
	}

	return activeTabWidget;
}

VDraggableTabWidget* VTabWidgetArea::makeVDraggableTabWidget( void )
{
	VDraggableTabWidget *newTabWidget = new VDraggableTabWidget();
	connect( newTabWidget , SIGNAL(widgetAdded(QWidget*)) , this , SLOT(tabWidgetTabAdded(QWidget*)) );
	connect( newTabWidget , SIGNAL(tabCountChanged(int)) , this , SLOT(tabWidgetCountChanged(int)) );
	connect( newTabWidget , SIGNAL(destroyed(QObject*)) , this , SLOT(removeTabWidgetFromLayout(QObject*)) );
	//connect( newTabWidget , SIGNAL(currentChanged(int)) , this , SLOT(tabWidgetCurrentChanged(int)) );
	connect( newTabWidget , SIGNAL(tabBarClicked(int)) , this , SLOT(tabWidgetCurrentChanged(int)) );

	//connect( newTabWidget , SIGNAL(tabCloseRequested(int)) , this , SLOT(tabCloseRequested(int)) );
	//connect( newTabWidget , SIGNAL(destructionRequested()) , this , SLOT(tabCloseRequested(int)) );

	return newTabWidget;
}

VDraggableTabWidget* VTabWidgetArea::getFirstTabWidget( QSplitter *splitter ) const
{
	if( splitter )
	{
		for( int index = 0 ; index < splitter->count() ; index++ )
		{
			VDraggableTabWidget *tabWidget = dynamic_cast< VDraggableTabWidget* >( splitter->widget( index ) );

			if( !tabWidget )
			{
				tabWidget = getFirstTabWidget( dynamic_cast< QSplitter* >( splitter->widget( index ) ) );

				if( tabWidget )
					return tabWidget;
			}
			else
				return tabWidget;
		}
	}

	return NULL;
}

void VTabWidgetArea::split( Qt::Orientation orientation )
{
	std::cout << QString( "void VTabWidgetArea::split( %1 )" ).arg( (int)orientation ).toLatin1().data() << std::endl;

	VDraggableTabWidget *tabWidget = getActiveTabWidget();

	//std::cout << QString( "%1 maps to %2" ).arg( (int)this->focusWidget() ).arg( (int)tabWidget ).toLatin1().data() << std::endl;

	if( tabWidget && tabWidget->count() > 1 )
	{
		QSplitter *tabSplitter = tabWidget->parentWidget();
		QWidget *widget = tabWidget->currentWidget();

		// Make a new tab widget
		VDraggableTabWidget *newTabWidget = makeVDraggableTabWidget();
		newTabWidget->addTab( widget , tabWidget->tabText( tabWidget->currentIndex() ) );
		tabMap[ widget ] = newTabWidget;

		// Change orientation
		if( tabSplitter == this && tabSplitter->count() == 1 )
			this->setOrientation( orientation );

		// Add the new tab widget to a splitter
		if( tabSplitter->orientation() == orientation )
			tabSplitter->addWidget( newTabWidget );
		else
		{
			int index = tabSplitter->indexOf( tabWidget );

			QSplitter *newSplitter = new QSplitter( orientation , NULL );
			newSplitter->addWidget( tabWidget );
			newSplitter->addWidget( newTabWidget );

			// This needs to be last otherwise the proportions get messy
			tabSplitter->insertWidget( index , newSplitter );
		}

		//widget->setFocus();
		activeTabWidget = newTabWidget;
	}
}

void VTabWidgetArea::tabWidgetTabAdded( QWidget *widget )
{
	std::cout << QString( "void VTabWidgetArea::tabWidgetTabAdded( %1 )" ).arg( (long)widget ).toLatin1().data() << std::endl;

	VDraggableTabWidget *tabWidget = dynamic_cast< VDraggableTabWidget* >( QObject::sender() );

	if( widget && tabWidget )
	{
		std::cout << QString( "%1 added to %2" ).arg( (long)widget ).arg( (long)tabWidget ).toLatin1().data() << std::endl;
		tabMap[ widget ] = tabWidget;
	}
}

void VTabWidgetArea::tabWidgetTabDestroyed( QObject *object )
{
	std::cout << QString( "void VTabWidgetArea::tabWidgetTabDestroyed( %1 )" ).arg( (long)object ).toLatin1().data() << std::endl;

	QWidget *widget = dynamic_cast< QWidget* >( object );

	if( widget )
	{
		tabMap.remove( widget );
		emit widgetDeleted( widget );
	}
}

void VTabWidgetArea::tabWidgetCountChanged( int count )
{
	std::cout << QString( "void VTabWidgetArea::tabWidgetCountChanged( %1 )" ).arg( count ).toLatin1().data() << std::endl;
	//std::cout << QString( "tabWidgetCountChanged( %1 )" ).arg( count ).toLatin1().data() << std::endl;

	if( !count )
	{
		// Delete the tab widget that had it's last tab removed
		VDraggableTabWidget *caller = dynamic_cast< VDraggableTabWidget* >( QObject::sender() );

		// Don't delete if the last tab widget open
		if( caller && ( caller->parentWidget() != this || this->count() > 1 ) )
			caller->deleteLater();
	}
}

void VTabWidgetArea::tabWidgetCurrentChanged( int index )
{
	std::cout << QString( "void VTabWidgetArea::tabWidgetCurrentChanged( %1 )" ).arg( index ).toLatin1().data() << std::endl;
	VDraggableTabWidget *caller = dynamic_cast< VDraggableTabWidget* >( QObject::sender() );

	if( caller )
	{
		activeTabWidget = caller;

		// This can cause segfaults
		//if( index >= 0 && index < caller->count() )
			//caller->widget( index )->setFocus();
	}
}

#if 0
void VTabWidgetArea::tabCloseRequested( int index )
{
	std::cout << QString( "void VTabWidgetArea::tabCloseRequested( %1 )" ).arg( index ).toLatin1().data() << std::endl;
	VDraggableTabWidget *tabWidget = dynamic_cast< VDraggableTabWidget* >( QObject::sender() );

	if( tabWidget )
	{
		//QWidget *widget = tabWidget->widget( index );
		VJsonForm *form = dynamic_cast< VJsonForm* >( tabWidget->widget( index ) );

		if( form->isWindowModified() )
		{
			switch( QMessageBox::question( this , this->windowTitle() , "Save changes?" , QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel , QMessageBox::Cancel ) )
			{
				case QMessageBox::Yes:
					form->save();
				case QMessageBox::No:
					break;
				default:
					return;
			}
		}

		form->deleteLater();
	}
}
#endif

void VTabWidgetArea::removeTabWidgetFromLayout( VDraggableTabWidget *tabWidget )
{
	std::cout << QString( "void VTabWidgetArea::removeTabWidgetFromLayout( %1 )" ).arg( (long)tabWidget ).toLatin1().data() << std::endl;
	//std::cout << QString( "removeTabWidgetFromLayout( %1 )" ).arg( (int)tabWidget ).toLatin1().data() << std::endl;

	if( tabWidget )
	{
		QSplitter *parentSplitter = dynamic_cast< QSplitter* >( tabWidget->parentWidget() );

		// Don't close top splitter
		if( parentSplitter && ( parentSplitter != this || parentSplitter->count() > 1 ) )
		{
			QWidget *currentChildWidget = tabWidget->currentWidget();
			// Get the thing on the other side of this split. We don't know what it is
			int targetIndex = parentSplitter->indexOf( tabWidget ) - 1;
			targetIndex = targetIndex < 0 ? 1 : targetIndex;
			VDraggableTabWidget* otherTabWidget = dynamic_cast< VDraggableTabWidget* >( parentSplitter->widget( targetIndex ) );
			QSplitter *otherSplitter = dynamic_cast< QSplitter* >( parentSplitter->widget( targetIndex ) );

			// If the other thing is a splitter, get the first tab widget of it
			if( !otherTabWidget && otherSplitter )
				otherTabWidget = getFirstTabWidget( otherSplitter );

			tabWidget->disconnect(); // Stop indirect recursion

			// Move remaining tabs from this tab to the next one
			while( tabWidget->count() )
			{
				tabMap[ tabWidget->widget( 0 ) ] = otherTabWidget;
				otherTabWidget->addTab( tabWidget->widget( 0 ) , tabWidget->tabText( 0 ) );
			}

			//std::cout << QString( "parentSplitter->count() == %1" ).arg( parentSplitter->count() ).toLatin1().data() << std::endl;

			// Rearrange splitters
			if( parentSplitter->count() == 2 && parentSplitter != this )
			{
				QSplitter *parentParentSplitter = (QSplitter*)( parentSplitter->parentWidget() );
				int index = parentParentSplitter->indexOf( parentSplitter );

				// Add the other widget to the parent layout
				if( otherSplitter )
				{
					// Add all of the other splitter's widgets to the splitter above that
					while( otherSplitter->count() )
						parentParentSplitter->insertWidget( index , otherSplitter->widget( otherSplitter->count() - 1 ) );

					otherSplitter->deleteLater();
				}
				else
					parentParentSplitter->insertWidget( index , otherTabWidget );

				parentSplitter->deleteLater();
			}
			//else // Multiple splits or this. Simply remove one

			tabWidget->deleteLater();

			if( currentChildWidget )
				currentChildWidget->setFocus();
		}
	}
}

void VTabWidgetArea::subWidgetTitleChanged( const QString &title )
{
	std::cout << QString( "void VTabWidgetArea::subWidgetTitleChanged( %1 )" ).arg( title ).toLatin1().data() << std::endl;

	QWidget *caller = dynamic_cast< QWidget* >( QObject::sender() );

	if( caller )
	{
		VDraggableTabWidget *tabWidget = tabMap.value( caller , NULL );

		if( tabWidget )
		{
			tabWidget->setTabText( tabWidget->indexOf( caller ) , title );
		}
	}
}

void VTabWidgetArea::subWidgetModifiedChange( QObject *obj , bool isModified )
{
	std::cout << (int)obj << " has been " << ( isModified ? "modified" : "unmodified" ) << std::endl;

	QWidget *widget = (QWidget*)obj;
	VDraggableTabWidget *tabWidget = tabMap.value( obj , NULL );

	if( tabWidget )
	{
		int index = tabWidget->indexOf( obj );

		if( index != -1 )
		{
			if( isModified )
				tabWidget->setTabText( index , widget->windowTitle().replace( "[*]" , "*" ) );
			else
				tabWidget->setTabText( index , widget->windowTitle().replace( "[*]" , "" ) );
		}
	}
}
