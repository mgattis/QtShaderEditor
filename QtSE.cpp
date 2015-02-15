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

#if 0
	editorArea = new QMdiArea( NULL );

	QTextEdit *testEdit = new QTextEdit( NULL );
	editorArea->addSubWindow( testEdit );
#endif

	VJsonForm *form = new VJsonForm( NULL );
	connect( form , SIGNAL(destroyed(QObject*)) , this , SLOT(tabWidgetDestroyed(QObject*)) );
	VJsonForm *form2 = new VJsonForm( NULL );
	connect( form2 , SIGNAL(destroyed(QObject*)) , this , SLOT(tabWidgetDestroyed(QObject*)) );
	VJsonForm *form3 = new VJsonForm( NULL );
	connect( form3 , SIGNAL(destroyed(QObject*)) , this , SLOT(tabWidgetDestroyed(QObject*)) );

	VDraggableTabWidget *initialTab = new VDraggableTabWidget();
	connect( initialTab , SIGNAL(widgetAdded(QWidget*)) , this , SLOT(tabWidgetAdded(QWidget*)) );
	//connect( initialTab , SIGNAL(widgetRemoved(QWidget*)) , this , SLOT(tabWidgetRemoved(QWidget*)) );
	initialTab->addTab( form , "Shader 1" );
	initialTab->addTab( form2 , "Shader 2" );
	initialTab->addTab( form3 , "Shader 3" );

	windowSplitter = new QSplitter( NULL );
	windowSplitter->addWidget( viewSplitter );
	windowSplitter->setStretchFactor( 0 , 1 );
	windowSplitter->addWidget( initialTab );
	windowSplitter->setStretchFactor( 1 , 4 );

	//splitterMap.insert( initialTab , windowSplitter );
	//tabMap.insert( form , initialTab );
	//tabMap.insert( form2 , initialTab );
	//tabMap.insert( form3 , initialTab );

	this->setCentralWidget( windowSplitter );
	form->setFocus();
}

QtSE::~QtSE()
{
	// Nothing to do
}

void QtSE::moveTabsFromTo( QTabWidget *src , QTabWidget *dst )
{
	while( src->count() )
	{
		tabMap[ src->widget( 0 ) ] = dst;
		dst->addTab( src->widget( 0 ) , src->tabText( 0 ) );
	}
}

VDraggableTabWidget* QtSE::getFirstTabWidget( QSplitter *splitter )
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

	/*
	if( !tabWidget )
		tabWidget = dynamic_cast< QTabWidget* >( this->focusWidget() );

	if( !tabWidget )
	{
		QTabBar *tabBar = dynamic_cast< QTabBar* >( this->focusWidget() );
		tabWidget = dynamic_cast< QTabWidget* >( tabBar->parentWidget() );
	}
	*/

	if( tabWidget && tabWidget->count() > 1 )
	{
		QSplitter *tabSplitter = tabWidget->parentWidget();
		VJsonForm *form = tabWidget->currentWidget();

		VDraggableTabWidget *newTabWidget = new VDraggableTabWidget();
		connect( newTabWidget , SIGNAL(widgetAdded(QWidget*)) , this , SLOT(tabWidgetAdded(QWidget*)) );
		//connect( newTabWidget , SIGNAL(widgetRemoved(QWidget*)) , this , SLOT(tabWidgetRemoved(QWidget*)) );

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

	if( tabWidget )
	{
		QSplitter *parentSplitter = (QSplitter*)tabWidget->parentWidget();

		// Don't close top splitter
		if( parentSplitter == windowSplitter )
			return;

		VJsonForm *form = tabWidget->currentWidget();
		VDraggableTabWidget* otherTabWidget = dynamic_cast< VDraggableTabWidget* >( parentSplitter->widget( !parentSplitter->indexOf( tabWidget ) ) );
		QSplitter* otherSplitter = dynamic_cast< QSplitter* >( parentSplitter->widget( !parentSplitter->indexOf( tabWidget ) ) );

		if( !otherTabWidget && otherSplitter )
			otherTabWidget = getFirstTabWidget( otherSplitter );

		if( otherTabWidget )
		{
#if 1
			if( parentSplitter->count() > 2 )
			{
				moveTabsFromTo( tabWidget , otherTabWidget );
				delete tabWidget;
			}
			else if( parentSplitter->count() == 2 )
			{
				moveTabsFromTo( tabWidget , otherTabWidget );
				delete tabWidget;

				if( otherSplitter )
					( (QSplitter*)( parentSplitter->parentWidget() ) )->addWidget( otherSplitter );
				else
					( (QSplitter*)( parentSplitter->parentWidget() ) )->addWidget( otherTabWidget );

				delete parentSplitter;
			}
#elif 0

#else
			if( parentSplitter->count() > 2 )
			{
				//int targetIndex = !parentSplitter->indexOf( tabWidget );
				//moveTabsFromTo( tabWidget , parentSplitter->widget( targetIndex ) );
				moveTabsFromTo( tabWidget , otherTabWidget );
				delete tabWidget;
			}
			else if( parentSplitter->count() == 2 )
			{
				//VDraggableTabWidget *otherTab = parentSplitter->widget( !parentSplitter->indexOf( tabWidget ) );
				//moveTabsFromTo( tabWidget , otherTab );
				moveTabsFromTo( tabWidget , otherTabWidget );
				delete tabWidget;
				//( (QSplitter*)( parentSplitter->parentWidget() ) )->addWidget( otherTab );
				( (QSplitter*)( parentSplitter->parentWidget() ) )->addWidget( otherTabWidget );
				delete parentSplitter;
			}
#endif

			form->setFocus();
		}
	}
}

void QtSE::tabWidgetAdded( QWidget *widget )
{
	VJsonForm *form = dynamic_cast< VJsonForm* >( widget );
	VDraggableTabWidget *tabWidget = dynamic_cast< VDraggableTabWidget* >( QObject::sender() );

	if( form && tabWidget )
	{
		std::cout << QString( "%1 added to %2" ).arg( (int)form ).arg( (int)tabWidget ).toLatin1().data() << std::endl;
		tabMap[ form ] = tabWidget;
	}
}

void QtSE::tabWidgetDestroyed( QObject *object )
{
	VJsonForm *form = dynamic_cast< VJsonForm* >( object );

	if( form )
	{
		std::cout << QString( "%1 removed" ).arg( (int)form ).toLatin1().data() << std::endl;
		tabMap.remove( form );
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
