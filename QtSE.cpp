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
	VJsonForm *form2 = new VJsonForm( NULL );
	VJsonForm *form3 = new VJsonForm( NULL );

	VDraggableTabWidget *initialTab = new VDraggableTabWidget();
	initialTab->setTabsClosable( true );
	//initialTab->setMovable( true );
	initialTab->addTab( form , "Shader 1" );
	initialTab->addTab( form2 , "Shader 2" );
	initialTab->addTab( form3 , "Shader 3" );

	windowSplitter = new QSplitter( NULL );
	windowSplitter->addWidget( viewSplitter );
	windowSplitter->setStretchFactor( 0 , 1 );
	windowSplitter->addWidget( initialTab );
	windowSplitter->setStretchFactor( 1 , 4 );

	//splitterMap.insert( initialTab , windowSplitter );
	tabMap.insert( form , initialTab );
	tabMap.insert( form2 , initialTab );
	tabMap.insert( form3 , initialTab );

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

		QTabWidget *newTabWidget = new QTabWidget( NULL );
		newTabWidget->setTabsClosable( true );
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

		if( parentSplitter->count() > 2 )
		{
			int targetIndex = !parentSplitter->indexOf( tabWidget );
			moveTabsFromTo( tabWidget , parentSplitter->widget( targetIndex ) );
			delete tabWidget;
		}
		else if( parentSplitter->count() == 2 )
		{
			QTabWidget *otherTab = parentSplitter->widget( !parentSplitter->indexOf( tabWidget ) );
			moveTabsFromTo( tabWidget , otherTab );
			delete tabWidget;
			( (QSplitter*)( parentSplitter->parentWidget() ) )->addWidget( otherTab );
			delete parentSplitter;
		}

		form->setFocus();
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
