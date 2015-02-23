#include "QtSE.h"

QtSE::QtSE( QWidget *parent ) : QMainWindow( parent )
{
	menuFile = this->menuBar()->addMenu( "File" );
		actionQuit = menuFile->addAction( "Quit" , this , SLOT(close()) );

	menuView = this->menuBar()->addMenu( "View" );
		actionSplitHorizontally = menuView->addAction( "Split Horizontal" , this , SLOT(splitHorizontally()) );
		actionSplitVertically = menuView->addAction( "Split Vertical" , this , SLOT(splitVertically()) );
		actionSplitCollapse = menuView->addAction( "Collapse" , this , SLOT(splitCollapse()) );

	menuHelp = this->menuBar()->addMenu( "Help" );
		actionAbout = menuHelp->addAction( "About" , this , SLOT(close()) );

	projectTree = new QTreeWidget( NULL );
	projectTree->header()->setStretchLastSection( true );
	projectTree->header()->setHidden( true );
	projectTree->setContextMenuPolicy( Qt::CustomContextMenu );
	connect( projectTree , SIGNAL(customContextMenuRequested(QPoint)) , this , SLOT(treeContextMenu(QPoint)) );

	viewWidget = new VGLView( NULL );

	CProjectTreeItem *dummyStage = new CProjectTreeItem( CProjectTreeItem::stage , projectTree , QStringList( "Stage 1" ) );
	CProjectTreeItem *dummyFB = new CProjectTreeItem( CProjectTreeItem::framebuffer , dummyStage , QStringList( "Framebuffer 1" ) );
	CProjectTreeItem *dummyShader = new CProjectTreeItem( CProjectTreeItem::shader , dummyFB , QStringList( "Shader 1" ) );
	CProjectTreeItem *dummyModel = new CProjectTreeItem( CProjectTreeItem::model , dummyShader  , QStringList( "Model 1" ) );
	CProjectTreeItem *dummyTexture = new CProjectTreeItem( CProjectTreeItem::texture , dummyModel  , QStringList( "Texture 1" ) );
	projectTree->expandAll();

	itemsTab = new QTabWidget( NULL );
	itemsTab->addTab( projectTree , "Project" );
	itemsTab->setTabPosition( QTabWidget::East );
	itemsTab->setTabsClosable( false );

	viewSplitter = new QSplitter( NULL );
	viewSplitter->setOrientation( Qt::Vertical );
	viewSplitter->addWidget( viewWidget );
	viewSplitter->setStretchFactor( 0 , 1 );
	viewSplitter->addWidget( itemsTab );
	viewSplitter->setStretchFactor( 1 , 4 );

	VDraggableTabWidget *initialTab = makeVDraggableTabWidget();

	// Make some test stuff
	for( int index = 0 ; index < 6 ; index++ )
		initialTab->addTab( makeVJsonForm() , QString( "Shader %1" ).arg( index + 1 ) );

	windowSplitter = new QSplitter( NULL );
	windowSplitter->addWidget( viewSplitter );
	windowSplitter->setStretchFactor( 0 , 1 );
	windowSplitter->addWidget( initialTab );
	windowSplitter->setStretchFactor( 1 , 4 );

	this->setCentralWidget( windowSplitter );
	initialTab->widget( 0 )->setFocus();
}

QtSE::~QtSE()
{
	// Nothing to do
}

VJsonForm* QtSE::makeVJsonForm( void )
{
	VJsonForm *form = new VJsonForm( NULL );
	connect( form , SIGNAL(destroyed(QObject*)) , this , SLOT(tabWidgetTabDestroyed(QObject*)) );

	return form;
}

VDraggableTabWidget* QtSE::makeVDraggableTabWidget( void )
{
	VDraggableTabWidget *newTabWidget = new VDraggableTabWidget();
	connect( newTabWidget , SIGNAL(widgetAdded(QWidget*)) , this , SLOT(tabWidgetTabAdded(QWidget*)) );
	connect( newTabWidget , SIGNAL(tabCountChanged(int)) , this , SLOT(tabWidgetCountChanged(int)) );
	connect( newTabWidget , SIGNAL(destroyed(QObject*)) , this , SLOT(removeTabWidgetFromLayout(QObject*)) );

	return newTabWidget;
}

VDraggableTabWidget* QtSE::getFirstTabWidget( QSplitter *splitter ) const
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

void QtSE::removeTabWidgetFromLayout( VDraggableTabWidget *tabWidget )
{
	std::cout << QString( "removeTabWidgetFromLayout( %1 )" ).arg( (int)tabWidget ).toLatin1().data() << std::endl;

	if( tabWidget )
	{
		QSplitter *parentSplitter = dynamic_cast< QSplitter* >( tabWidget->parentWidget() );

		// Don't close top splitter
		if( !parentSplitter || parentSplitter == windowSplitter )
			return;

		VJsonForm *form = tabWidget->currentWidget();
		VDraggableTabWidget* otherTabWidget = dynamic_cast< VDraggableTabWidget* >( parentSplitter->widget( !parentSplitter->indexOf( tabWidget ) ) );
		QSplitter *otherSplitter = dynamic_cast< QSplitter* >( parentSplitter->widget( !parentSplitter->indexOf( tabWidget ) ) );

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

			if( form )
				form->setFocus();
		}
	}
}

void QtSE::treeContextMenu( QPoint point )
{
	CProjectTreeItem *item = (CProjectTreeItem*)projectTree->itemAt( point );
	QMenu *menu = new QMenu();
	menu->setAttribute( Qt::WA_DeleteOnClose , true );

	if( item )
	{
		switch( item->getPartType() )
		{
			case CProjectTreeItem::stage:
				menu->addAction( "Add Framebuffer" , this , SLOT(addFramebuffer()) );
				break;
			case CProjectTreeItem::framebuffer:
				menu->addAction( "Add Shader" , this , SLOT(addShader()) );
				break;
			case CProjectTreeItem::shader:
				menu->addAction( "Add Model" , this , SLOT(addModel()) );
				menu->addAction( "Add Texture" , this , SLOT(addTexture()) );
				break;
			case CProjectTreeItem::texture:
				//menu->addAction( "Add Stage" );
				return;
				break;
			case CProjectTreeItem::model:
				menu->addAction( "Add Texture" , this , SLOT(addTexture()) );
				break;
			default: {}
		}
	}
	else
	{
		menu->addAction( "Add Stage" , this , SLOT(addStage()) );
	}

	menu->popup( QCursor::pos() , NULL );
}

void QtSE::split( Qt::Orientation orientation )
{
	QTabWidget *tabWidget = tabMap.value( this->focusWidget() , NULL );

	if( tabWidget && tabWidget->count() > 1 )
	{
		QSplitter *tabSplitter = tabWidget->parentWidget();
		VJsonForm *form = tabWidget->currentWidget();

		VDraggableTabWidget *newTabWidget = makeVDraggableTabWidget();
		newTabWidget->addTab( form , tabWidget->tabText( tabWidget->currentIndex() ) );
		tabMap[ form ] = newTabWidget;

		if( tabSplitter != windowSplitter && tabSplitter->orientation() == orientation )
			tabSplitter->addWidget( newTabWidget );
		else
		{
			QSplitter *newSplitter = new QSplitter( orientation , NULL );
			newSplitter->addWidget( tabWidget );
			newSplitter->addWidget( newTabWidget );

			tabSplitter->addWidget( newSplitter );
		}

		form->setFocus();
	}
}

void QtSE::splitCollapse( void )
{
	QTabWidget *tabWidget = tabMap.value( this->focusWidget() , NULL );

	removeTabWidgetFromLayout( tabWidget );
}

void QtSE::tabWidgetTabAdded( QWidget *widget )
{
	VJsonForm *form = dynamic_cast< VJsonForm* >( widget );
	VDraggableTabWidget *tabWidget = dynamic_cast< VDraggableTabWidget* >( QObject::sender() );

	if( form && tabWidget )
	{
		//std::cout << QString( "%1 added to %2" ).arg( (int)form ).arg( (int)tabWidget ).toLatin1().data() << std::endl;
		tabMap[ form ] = tabWidget;
	}
}

void QtSE::tabWidgetTabDestroyed( QObject *object )
{
	VJsonForm *form = dynamic_cast< VJsonForm* >( object );

	if( form )
	{
		//std::cout << QString( "%1 removed" ).arg( (int)form ).toLatin1().data() << std::endl;
		tabMap.remove( form );
	}
}

void QtSE::tabWidgetCountChanged( int count )
{
	//std::cout << QString( "tabWidgetCountChanged( %1 )" ).arg( count ).toLatin1().data() << std::endl;

	if( !count )
	{
		// Delete the tab widget that had it's last tab removed
		VDraggableTabWidget *caller = dynamic_cast< VDraggableTabWidget* >( QObject::sender() );

		if( caller )
			caller->deleteLater();
	}
}

void QtSE::addStage( void )
{
	//
}

void QtSE::addFramebuffer( void )
{
	//
}

void QtSE::addShader( void )
{
	//
}

void QtSE::addModel( void )
{
	//
}

void QtSE::addTexture( void )
{
	//
}
