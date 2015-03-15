#include "VTabWidgetArea.h"

VTabWidgetArea::VTabWidgetArea()
{
	VDraggableTabWidget *initialTab = makeVDraggableTabWidget();

	for( int index = 0 ; index < 3 ; index++ )
	{
		initialTab->addTab( makeVJsonForm() , QString( "Test %1" ).arg( index + 1 ) );
		tabMap[ initialTab->widget( index ) ] = initialTab;
	}

	this->addWidget( initialTab );
}

VTabWidgetArea::~VTabWidgetArea()
{

}

VJsonForm* VTabWidgetArea::makeVJsonForm( void )
{
	VJsonForm *form = new VJsonForm( NULL );
	//connect( form , SIGNAL(destroyed(QObject*)) , this , SLOT(tabWidgetTabDestroyed(QObject*)) );
	//connect( form , SIGNAL(modified()) , this , SLOT(formModified()) );
	//connect( form , SIGNAL(unmodified()) , this , SLOT(formUnmodified()) );

	return form;
}

VDraggableTabWidget* VTabWidgetArea::makeVDraggableTabWidget( void )
{
	VDraggableTabWidget *newTabWidget = new VDraggableTabWidget();
	connect( newTabWidget , SIGNAL(widgetAdded(QWidget*)) , this , SLOT(tabWidgetTabAdded(QWidget*)) );
	connect( newTabWidget , SIGNAL(tabCountChanged(int)) , this , SLOT(tabWidgetCountChanged(int)) );
	connect( newTabWidget , SIGNAL(destroyed(QObject*)) , this , SLOT(removeTabWidgetFromLayout(QObject*)) );
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

	QTabWidget *tabWidget = tabMap.value( this->focusWidget() , NULL );
	std::cout << QString( "%1 maps to %2" ).arg( (int)this->focusWidget() ).arg( (int)tabWidget ).toLatin1().data() << std::endl;

	if( tabWidget && tabWidget->count() > 1 )
	{
		QSplitter *tabSplitter = tabWidget->parentWidget();
		QWidget *widget = tabWidget->currentWidget();
		//int index = tabWidget->currentIndex();

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
			QSplitter *newSplitter = new QSplitter( orientation , NULL );
			newSplitter->addWidget( tabWidget );
			newSplitter->addWidget( newTabWidget );

			//tabSplitter->insertWidget( index , newSplitter );
			tabSplitter->addWidget( newSplitter );
		}

		widget->setFocus();
	}
}

void VTabWidgetArea::splitCollapse( void )
{
	QTabWidget *tabWidget = tabMap.value( this->focusWidget() , NULL );

	removeTabWidgetFromLayout( tabWidget );
}

void VTabWidgetArea::tabWidgetTabAdded( QWidget *widget )
{
	std::cout << QString( "void VTabWidgetArea::tabWidgetTabAdded( %1 )" ).arg( (int)widget ).toLatin1().data() << std::endl;

	VDraggableTabWidget *tabWidget = dynamic_cast< VDraggableTabWidget* >( QObject::sender() );

	if( widget && tabWidget )
	{
		std::cout << QString( "%1 added to %2" ).arg( (int)widget ).arg( (int)tabWidget ).toLatin1().data() << std::endl;
		tabMap[ widget ] = tabWidget;
	}
}

void VTabWidgetArea::tabWidgetTabDestroyed( QObject *object )
{
	std::cout << QString( "void VTabWidgetArea::tabWidgetTabDestroyed( %1 )" ).arg( (int)object ).toLatin1().data() << std::endl;

	QWidget *widget = dynamic_cast< QWidget* >( object );

	if( widget )
	{
		//std::cout << QString( "%1 removed" ).arg( (int)widget ).toLatin1().data() << std::endl;
		tabMap.remove( widget );
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

void VTabWidgetArea::removeTabWidgetFromLayout( VDraggableTabWidget *tabWidget )
{
	std::cout << QString( "void VTabWidgetArea::removeTabWidgetFromLayout( %1 )" ).arg( (int)tabWidget ).toLatin1().data() << std::endl;
	//std::cout << QString( "removeTabWidgetFromLayout( %1 )" ).arg( (int)tabWidget ).toLatin1().data() << std::endl;

	if( tabWidget )
	{
		QSplitter *parentSplitter = dynamic_cast< QSplitter* >( tabWidget->parentWidget() );

		// Don't close top splitter
		if( parentSplitter && ( parentSplitter != this || parentSplitter->count() > 1 ) )
		{
			QWidget *currentChildWidget = tabWidget->currentWidget();
			// Get the thing on the other side of this split. We don't know what it is
			VDraggableTabWidget* otherTabWidget = dynamic_cast< VDraggableTabWidget* >( parentSplitter->widget( !parentSplitter->indexOf( tabWidget ) ) );
			QSplitter *otherSplitter = dynamic_cast< QSplitter* >( parentSplitter->widget( !parentSplitter->indexOf( tabWidget ) ) );

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

				// Add the other widget to the parent layout
				if( otherSplitter )
				{
					// windowSplitter should contain all the tab widgets in a single, child splitter
					// It may have other splits for different components
					//if( parentParentSplitter != windowSplitter && parentSplitter->orientation() == parentParentSplitter->orientation() )
					if( parentParentSplitter != this && otherSplitter->orientation() == parentParentSplitter->orientation() )
					{
						// Add all of the other splitter's widgets to the splitter above that
						while( otherSplitter->count() )
							parentParentSplitter->addWidget( otherSplitter->widget( 0 ) );
					}
					else
						parentParentSplitter->addWidget( otherSplitter );
				}
				else
					parentParentSplitter->addWidget( otherTabWidget );

				parentSplitter->deleteLater();
			}
			else // Multiple splits. Simply remove one
				tabWidget->deleteLater();

			if( currentChildWidget )
				currentChildWidget->setFocus();
		}
	}

#if 0
	std::cout << QString( "removeTabWidgetFromLayout( %1 )" ).arg( (int)tabWidget ).toLatin1().data() << std::endl;

	if( tabWidget )
	{
		QSplitter *parentSplitter = dynamic_cast< QSplitter* >( tabWidget->parentWidget() );

		// Don't close top splitter
		if( !parentSplitter || ( parentSplitter == this && parentSplitter->count() == 1 ) )
			return;

		QWidget *childWidget = tabWidget->currentWidget();
		// Get the thing on the other side of this split. We don't know what it is
		VDraggableTabWidget* otherTabWidget = dynamic_cast< VDraggableTabWidget* >( parentSplitter->widget( !parentSplitter->indexOf( tabWidget ) ) );
		QSplitter *otherSplitter = dynamic_cast< QSplitter* >( parentSplitter->widget( !parentSplitter->indexOf( tabWidget ) ) );

		// TODO: What if otherTabWidget is tabWidget?
		if( !otherTabWidget && otherSplitter )
			otherTabWidget = getFirstTabWidget( otherSplitter );

		if( otherTabWidget && parentSplitter->count() > 1 )
		{
			tabWidget->disconnect(); // Stop indirect recursion

			// Move remaining tabs from this tab to the next one
			while( tabWidget->count() )
				otherTabWidget->addTab( tabWidget->widget( 0 ) , tabWidget->tabText( 0 ) );

			//std::cout << QString( "parentSplitter->count() == %1" ).arg( parentSplitter->count() ).toLatin1().data() << std::endl;

			if( parentSplitter->count() == 2 )
			{
				QSplitter *parentParentSplitter = (QSplitter*)( parentSplitter->parentWidget() );

				// Add the other widget to the parent layout
				if( otherSplitter )
				{
					// windowSplitter should contain all the tab widgets in a single, child splitter
					// It may have other splits for different components
					//if( parentParentSplitter != windowSplitter && parentSplitter->orientation() == parentParentSplitter->orientation() )
					if( parentParentSplitter != windowSplitter && otherSplitter->orientation() == parentParentSplitter->orientation() )
					{
						// Add all of the other splitter's widgets to the splitter above that
						while( otherSplitter->count() )
							parentParentSplitter->addWidget( otherSplitter->widget( 0 ) );
					}
					else
						parentParentSplitter->addWidget( otherSplitter );
				}
				else
					parentParentSplitter->addWidget( otherTabWidget );

				parentSplitter->deleteLater();
			}
			else // Multiple splits. Simply remove one
				tabWidget->deleteLater();

			if( childWidget )
				childWidget->setFocus();
		}
	}
#endif
}
